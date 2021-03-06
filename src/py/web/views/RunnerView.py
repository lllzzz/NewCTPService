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
from flask_admin import BaseView,expose

from flask import Response, redirect, request,send_from_directory, render_template
import flask
import subprocess
import os
from common.Config import Config
from flask_apscheduler import APScheduler
from common.Locker import Locker
import datetime

import subprocess

class RunnerView(BaseView):

    def is_visible(self):
        return False


    @expose('/', methods=('POST', 'GET'))
    @expose('/test', methods=('POST', 'GET'))
    def test(self):

        if request.method == 'POST':

            id = request.args.get('id')
            model = Model.query.get(id)
            modelName = model.nick_name
            f = request.files['file']
            uploadPath = ''
            config = Config.get()
            if f.filename:
                uploadPath = config['appRoot'] + '/src/py/web/static/test_data/' + modelName + '.csv'
                f.save(uploadPath)

            @flask.stream_with_context
            def generate():
                id = request.args.get('id')
                model = Model.query.get(id)
                cmd = 'CTP_CONFIG_PATH=%s python %s/src/py/test.py %s %s' % (os.environ.get('CTP_CONFIG_PATH'), config['appRoot'], modelName, uploadPath)
                p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                while p.poll() is None:
                    line = p.stdout.read()
                    if not line: continue
                    yield line + '\n'

            return self.render('admin/runner_test.html', logs=generate())

        else:
            return self.render('admin/runner_test.html', logs=[])


    @expose('/lbtest', methods=['POST', 'GET'])
    def lbtest(self):
        config = Config.get()

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
            

            cmd = 'CTP_CONFIG_PATH=%s python %s/src/py/lbtest.py %s %s %s %s %s %s' \
                % (os.environ.get('CTP_CONFIG_PATH'), config['appRoot'], optional, modelName, testVersion, iids,
                startDate, endDate)

            now = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
            fd = open('%s/src/py/web/static/logs/lbtest/%s_%s_%s.log' % (config['appRoot'], modelName, testVersion, now), 'w')
            subprocess.Popen(cmd, shell=True, stdout=fd)

        locking = Locker.getLocking('TEST_SERVICE_RUNNING_' + modelName)
        files = []
        for file in os.listdir('%s/src/py/web/static/logs/lbtest/' % (config['appRoot'])):
            if file.find(modelName) >= 0:
                files.append(file)


        return self.render('admin/runner_lbtest.html', locking=locking, id=id, logs=files)

