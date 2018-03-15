#!/usr/bin/env python
# -*- encoding:utf-8 -*-
import sys
reload(sys)  
sys.setdefaultencoding('utf8')

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
    """
    模型类：
    编写算法时，需要重写onTick、tradeSuccess以及tradeCancel方法
    onTick：当有Tick信号时触发
    tradeSuccess：下单成功时触发
    tradeCancel：订单撤销时触发

    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    可用变量：
    self.appConfig: 应用配置，在Config配置中，appKey所对应的配置项

    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    可用方法：
    self.tradeNormal(iid='', price=3690, volume=2, isOpen=True, isBuy=True, isToday=True)
    该方法用于下普通单，直到成交，触发tradeSuccess方法，
    可手动撤单，撤单成功后会触发tradeCancel方法

    self.tradeFAK(iid='', price=3690, volume=2, isOpen=True, isBuy=True, isToday=True)
    该方法用于下FAK单，触发tradeSuccess或tradeCancel方法

    self.tradeFOK(iid='', price=3690, volume=2, isOpen=True, isBuy=True, isToday=True)
    该方法用于下FOK单，触发tradeSuccess或tradeCancel方法

    self.tradeIOC(iid='', price=3690, volume=2, isOpen=True, isBuy=True, isToday=True)
    该方法用于下市价单，涨跌停情况外，触发tradeSuccess方法

    以上全部方法均返回tradeId，tradeId为此次交易的唯一订单ID，
    可用于追踪订单

    self.cancel(iid, tradeId)
    该方法用于撤单

    """
    def __init__(self, appKey, tradeSrv):
        super(%s, self).__init__(appKey, tradeSrv)


    def onTick(self, tick):
        """
        tick：Dict
        {
            'iid': 'hc1808', // 合约
            'price': 3400, // 最新价
            'volume': 340, // 数量
            'bid1Price': 3333, 
            'bid1Volume': 300,
            'ask1Price': 3322,
            'ask1Volume': 33,
            'time': '20180312 11:23:44', // 时间
            'msec': 500, // 毫秒
        }
        """
        pass


    def tradeSuccess(self, tradeId, dealPrice, dealVolume):
        """
        tradeId: String 订单号
        dealPrice: Float 成交价
        dealVolume: Int 成交量
        """
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
