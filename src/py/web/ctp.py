from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from flask_admin import Admin,BaseView,expose,AdminIndexView
from flask_basicauth import BasicAuth
import os
from common.Config import Config

app = Flask(__name__)

app.config['SESSION_TYPE'] = 'filesystem'
app.config['SECRET_KEY'] = os.urandom(24)

app.config['BASIC_AUTH_USERNAME'] = 'admin'
app.config['BASIC_AUTH_PASSWORD'] = 'admin'
app.config['BASIC_AUTH_FORCE'] = True
BasicAuth(app)

config = Config.get()
app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql://%s:%s@%s/%s' % (config['mysql']['user'], config['mysql']['passwd'], config['mysql']['host'], config['mysql']['name'])
app.config['SQLALCHEMY_COMMIT_ON_TEARDOWN'] = True
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)


admin = Admin(app, name='CTP')


def stream_template(template_name, **context):
    app.update_template_context(context)
    template = app.jinja_env.get_template(template_name)
    generator = template.stream(context)
    # generator.enable_buffering(2)
    return generator
