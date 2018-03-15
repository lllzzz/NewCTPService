from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from flask_admin import Admin,BaseView,expose,AdminIndexView
from flask_basicauth import BasicAuth
import os

app = Flask(__name__)

app.config['SESSION_TYPE'] = 'filesystem'
app.config['SECRET_KEY'] = os.urandom(24)

app.config['BASIC_AUTH_USERNAME'] = 'admin'
app.config['BASIC_AUTH_PASSWORD'] = 'admin'
app.config['BASIC_AUTH_FORCE'] = True
BasicAuth(app)

app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql://root:Abc518131!@127.0.0.1/ctp_dev'
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
