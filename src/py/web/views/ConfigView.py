from flask_admin import BaseView,expose
from flask_admin.contrib.sqla import ModelView
from common.Config import Config
from flask import request, flash, redirect, Response
import json

class ConfigView(BaseView):
    @expose('/', methods=('GET', 'POST'))
    def index(self):
        config = Config.get()
        app = config['app']
        appStr = json.dumps(app, indent=4)

        if request.method == 'POST':
            content = request.form.get('config')
            newApp = json.loads(content)
            config['app'] = newApp
            Config.write(config)
            return redirect('admin/config')

        return self.render('admin/config.html', appStr = appStr)
