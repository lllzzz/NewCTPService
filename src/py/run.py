from web.ctp import app, db, admin, stream_template
from flask_admin.contrib.sqla import ModelView
from web.models.TickHc import TickHc
from web.models.Model import Model
from web.views.CtpModelView import CtpModelView
from web.views.TickView import TickView
from web.views.ConfigView import ConfigView
from web.views.LBTestView import LBTestView
from redis import Redis
from flask_admin.contrib import rediscli

from flask import Response, redirect, request,send_from_directory
import flask
import subprocess
import os
from common.Config import Config

from flask_apscheduler import APScheduler

config = Config.get()
admin.add_view(CtpModelView(Model, db.session))
admin.add_view(TickView(TickHc, db.session, category='Tick'))
admin.add_view(ConfigView(name='Config'))
admin.add_view(LBTestView(name='LBTest'))
admin.add_view(rediscli.RedisCli(Redis(host=config['redis']['host'], db=config['redis']['db'])))

@app.route('/admin/model/test/', methods=['POST', 'GET'])
def test():

    if request.method == 'POST':

        id = request.args.get('id')
        model = Model.query.get(id)
        className = model.class_name
        f = request.files['file']
        if f.filename:
            uploadPath = 'src/py/web/static/test_data/' + className + '.csv'
            f.save(uploadPath)

        @flask.stream_with_context
        def generate():
            id = request.args.get('id')
            model = Model.query.get(id)
            cmd = 'CTP_CONFIG_PATH=%s python src/py/test.py test model.%s %s' % (os.environ.get('CTP_CONFIG_PATH'), model.class_name, model.class_name)
            p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            while p.poll() is None:
                line = p.stdout.read()
                if not line: continue
                yield line + '\n'

        return Response(stream_template('admin/model_test.html', logs=generate()))

    else:
        return Response(stream_template('admin/model_test.html', logs=[]))

app.run(debug=True, threaded=True)