from flask_admin import BaseView,expose
from flask_admin.contrib.sqla import ModelView
from common.Config import Config
from flask import request, flash, redirect, Response, jsonify
import json
from web.models.Service import Service

class ServiceView(ModelView):

    # def is_visible(self):
    #     return False

    @expose('/', methods=('GET', 'POST'))
    def index(self):

        services = Service.query.all()
        return self.render('admin/service.html', services=services)

    @expose('/remove', methods=['POST'])
    def remove(self):

        id = request.form.get('id')

        if id:
            service = self.get_one(id)
            if service is None:
                return jsonify(code=10001, msg="service is not exist")

            if self.delete_model(service):
                return jsonify(code=0, msg="ok")

        return jsonify(code=10002, msg="system error")


    @expose('/save', methods=['POST'])
    def save(self):

        id = request.form.get('id')

        if id:
            service = self.get_one(id)
            if service is None:
                return jsonify(code=10001, msg="service is not exist")
            form = self.edit_form(obj=service)
            if self.update_model(form, service):
                return jsonify(code=0, msg="ok")

        else:
            form = self.edit_form()
            if self.create_model(form):
                return jsonify(code=0, msg="ok")

        return jsonify(code=10002, msg="system error")
