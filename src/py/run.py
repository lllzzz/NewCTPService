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

# import custom view
import web.views.RunnerView

config = Config.get()
admin.add_view(CtpModelView(Model, db.session))
admin.add_view(TickData(name='Upload Data', category='Tick'))
admin.add_view(ConfigView(name='Config'))
# admin.add_view(LBTestView(name='LBTest'))
admin.add_view(rediscli.RedisCli(Redis(host=config['redis']['host'], db=config['redis']['db'])))

# add tick class from config
for iid in config['iids']:
    tickModel = createTickModel(iid)
    admin.add_view(TickView(tickModel, db.session, category='Tick'))


app.run(debug=True, threaded=True)
