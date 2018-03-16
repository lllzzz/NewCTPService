#!/usr/bin/env python
# -*- encoding:utf-8 -*-


from flask_admin import BaseView,expose
from flask_admin.contrib.sqla import ModelView
from flask import request, flash, redirect, Response, send_from_directory
from flask_admin.helpers import get_redirect_target
from web.ctp import stream_template, app
import subprocess
import os
from common.Config import Config
import datetime

class LBTestView(BaseView):

    @expose('/', methods=('GET', 'POST'))
    def lbtest(self):

        files = []
        for file in os.listdir('src/py/web/static/logs/'):
            if file == '.gitignore': continue
            files.append(file)

        return self.render('admin/lbtest.html', files=files)


    @expose('/run/', methods=('GET', 'POST'))
    def lbtest_run(self):

        app = request.form.get('app')
        config = Config.get()

        if not config['app'].has_key(app):
            flash('Please add config', 'error')
            return redirect('/admin/model/lbtest/')

        now = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
        cmd = 'CTP_CONFIG_PATH=%s python src/py/lbtest.py %s' % (os.environ.get('CTP_CONFIG_PATH'), app)
        fd = open('src/py/web/static/logs/lbtest_%s_%s.log' % (app, now), 'w')
        subprocess.Popen(cmd, shell=True, stdout=fd)
        flash('Test is Runing', 'success')
        return redirect('admin/lbtestview/')
