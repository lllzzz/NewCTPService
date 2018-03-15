from flask_admin import BaseView,expose
from flask_admin.contrib.sqla import ModelView
from flask import request, flash, redirect, Response
from flask_admin.helpers import get_redirect_target
from web.ctp import stream_template, app
import subprocess

class CtpModelView(ModelView):


    create_template = 'admin/model_create.html'
    edit_template = 'admin/model_edit.html'


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
