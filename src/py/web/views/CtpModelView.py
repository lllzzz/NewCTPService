from flask_admin import BaseView,expose
from flask_admin.contrib.sqla import ModelView
from flask import request, flash, redirect, Response, send_from_directory
from flask_admin.helpers import get_redirect_target
from web.ctp import stream_template, app
import subprocess
import os
from common.Config import Config
import datetime

class CtpModelView(ModelView):


    create_template = 'admin/model_create.html'
    edit_template = 'admin/model_edit.html'


    @expose('/lbtest/', methods=('GET', 'POST'))
    def lbtest(self):

        files = []
        for file in os.listdir('src/py/web/static/logs/'):
            if file == '.gitignore': continue
            files.append(file)

        return self.render('admin/lbtest.html', files=files)


    @expose('/lbtest/run/', methods=('GET', 'POST'))
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
        return redirect('/admin/model/lbtest/')


    @expose('/edit/', methods=('GET', 'POST'))
    def edit_view(self):

        id = request.args.get('id')
        return_url = get_redirect_target() or self.get_url('.index_view')

        model = self.get_one(id)
        if model is None:
            flash('Record does not exist.', 'error')
            return redirect(return_url)

        form = self.edit_form(obj=model)
        if self.validate_form(form):
            if self.update_model(form, model):

                filename = 'src/py/model/%s.py' % (form.class_name.data)

                with open(filename,'w') as f:
                    f.write(form.source.data)

                flash('Record was successfully saved.', 'success')
                return redirect(request.url)

        if request.method == 'GET':
            self.on_form_prefill(form, id)

        if not form.source.data:
            form.source.data = '''
#!/usr/bin/env python
# -*- encoding:utf-8 -*-
#
from .ModelBase import ModelBase

class %s(ModelBase):
    """docstring for DemoModel"""
    def __init__(self, appFrom, tradeSrv):
        super(%s, self).__init__(appFrom, tradeSrv)


    def onTick(self, tick):
        pass


    def tradeSuccess(self, tradeId, dealPrice, dealVolume):
        pass

    def tradeCancel(self, tradeId):
        pass
''' % (form.class_name.data, form.class_name.data)

        template = self.edit_template

        return self.render(template,
                           id = id,
                           model=model,
                           form=form,
                           return_url=return_url)
