from web.ctp import app, db, admin, stream_template
from flask_admin.contrib.sqla import ModelView
from web.models.Tick import createTickModel
from web.models.Model import Model
from web.models.Trade import Trade
from web.models.Service import Service
from web.views.CtpModelView import CtpModelView
from web.views.TickView import *
from web.views.ConfigView import ConfigView
from web.views.ServiceView import ServiceView
from web.views.RunnerView import RunnerView
from web.views.TradeView import TradeView
from redis import Redis
from flask_admin.contrib import rediscli
from common.Config import Config

# import custom view
# import web.views.RunnerView

config = Config.get()
admin.add_view(CtpModelView(Model, db.session))
admin.add_view(TickData(name='Upload Data', category='Tick'))
admin.add_view(TradeView(Trade, db.session, name='Trade'))
admin.add_view(ConfigView(name='Config', url="/admin/config"))
admin.add_view(ServiceView(Service, db.session, name='Service'))
admin.add_view(RunnerView(name='Runner', url="/admin/runner"))
admin.add_view(rediscli.RedisCli(Redis(host=config['redis']['host'], db=config['redis']['db'])))

# add tick class from config
for iid in config['iids']:
    tickModel = createTickModel(iid)
    admin.add_view(TickView(tickModel, db.session, category='Tick'))


app.run(debug=True, threaded=True)
