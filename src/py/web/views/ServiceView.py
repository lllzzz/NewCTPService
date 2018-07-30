from flask_admin import BaseView,expose
from flask_admin.contrib.sqla import ModelView
from common.Config import Config
from flask import request, flash, redirect, Response
import json
from web.models.Service import Service

class ServiceView(BaseView):
    
    # def is_visible(self):
    #     return False

    @expose('/', methods=('GET', 'POST'))
    def index(self):
        
        services = Service.query.all()
        print services



        return self.render('admin/service.html', services=services)
