from web.ctp import app, db, admin, stream_template
from flask_admin.contrib.sqla import ModelView
from web.models.Tick import createTickModel
from web.models.Model import Model
from web.views.CtpModelView import CtpModelView
from web.views.TickView import *
from web.views.ConfigView import ConfigView
from web.views.LBTestView import LBTestView
from redis import Redis
from flask_admin.contrib import rediscli
from common.Config import Config

from flask import Response, redirect, request,send_from_directory, render_template
import flask
import subprocess
import os
from common.Config import Config
from flask_apscheduler import APScheduler
from common.Locker import Locker
import datetime

import subprocess

@app.route('/admin/model/test/', methods=['POST', 'GET'])
def test():

    if request.method == 'POST':

        id = request.args.get('id')
        model = Model.query.get(id)
        modelName = model.nick_name
        f = request.files['file']
        uploadPath = ''
        if f.filename:
            config = Config.get()
            uploadPath = config + 'src/py/web/static/test_data/' + modelName + '.csv'
            f.save(uploadPath)

        @flask.stream_with_context
        def generate():
            id = request.args.get('id')
            model = Model.query.get(id)
            cmd = 'CTP_CONFIG_PATH=%s python src/py/test.py %s %s' % (os.environ.get('CTP_CONFIG_PATH'), modelName, uploadPath)
            p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            while p.poll() is None:
                line = p.stdout.read()
                if not line: continue
                yield line + '\n'

        return Response(stream_template('admin/model_test.html', logs=generate()))

    else:
        return Response(stream_template('admin/model_test.html', logs=[]))


@app.route('/admin/model/lbtest/', methods=['POST', 'GET'])
def lbtest():
    
    if request.method == 'GET':

        id = request.args.get('id')
        model = Model.query.get(id)
        modelName = model.nick_name
        
    else:

        id = request.form.get('id')
        testVersion = request.form.get('testVersion')
        iids = request.form.get('iids')
        startDate = request.form.get('startDate')
        endDate = request.form.get('endDate')
        isRandom = request.form.get('isRandom')
        maxSlippage = request.form.get('maxSlippage')

        optional = ''
        if isRandom:
            optional += ' -r '

        if isRandom and maxSlippage:
            optional += ' -s ' + maxSlippage + ' '

        model = Model.query.get(id)
        modelName = model.nick_name

        cmd = 'CTP_CONFIG_PATH=%s python src/py/lbtest.py %s %s %s %s %s %s' \
            % (os.environ.get('CTP_CONFIG_PATH'), optional, modelName, testVersion, iids, 
            startDate, endDate)

        now = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
        fd = open('src/py/web/static/logs/lbtest_%s_%s_%s.log' % (modelName, testVersion, now), 'w')
        subprocess.Popen(cmd, shell=True, stdout=fd)

    locking = Locker.getLocking('TEST_SERVICE_RUNNING_')
    files = []
    for file in os.listdir('src/py/web/static/logs/'):
        if file.find(modelName) > 0:
            files.append(file)


    return render_template('admin/model_lbtest.html', locking=locking, id=id, logs=files)
