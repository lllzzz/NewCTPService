from flask_admin import BaseView,expose
from flask_admin.contrib.sqla import ModelView
from flask import request, flash, redirect, Response
from common.Config import Config
from common.MySQL import MySQL
from web.ctp import app, db, admin, stream_template
from web.models.Tick import createTickModel
from flask import jsonify
import csv
import hashlib

class TickView(ModelView):
    can_delete = False
    can_create = False
    can_edit = False
    can_view_details = True


class TickNew(BaseView):

    @expose('/', methods=('GET', 'POST'))
    def index(self):


        if request.method == 'POST':
            config = Config.get()
            app = config['app']

            tick = request.form.get('tick')

            if tick not in config['iids']:
                config['iids'].append(tick)
                Config.write(config)

                mysql = MySQL()
                sql = '''
                    CREATE TABLE IF NOT EXISTS `tick_%s` (
                        `id` bigint(20) NOT NULL AUTO_INCREMENT,
                        `token` char(32) NOT NULL DEFAULT '',
                        `iid` varchar(50) NOT NULL DEFAULT '',
                        `time` char(17) NOT NULL DEFAULT '',
                        `msec` int(11) NOT NULL DEFAULT '0',
                        `price` decimal(10,2) NOT NULL DEFAULT '0.00',
                        `volume` int(11) NOT NULL DEFAULT '0',
                        `bid_price1` decimal(10,2) NOT NULL DEFAULT '0.00',
                        `bid_volume1` int(11) NOT NULL DEFAULT '0',
                        `ask_price1` decimal(10,2) NOT NULL DEFAULT '0.00',
                        `ask_volume1` int(11) NOT NULL DEFAULT '0',
                        `mtime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
                        PRIMARY KEY (`id`),
                        UNIQUE KEY idx_token(`token`),
                        KEY idx_time(`time`)
                        ) ENGINE=InnoDB CHARSET=utf8;''' % (tick)
                mysql.insert(sql)
                tickModel = createTickModel(tick)
                admin.add_view(TickView(tickModel, db.session, category='Tick'))


            return self.render('admin/tick_new.html')

        return self.render('admin/tick_new.html')





class TickData(BaseView):

    @expose('/', methods=('GET', 'POST'))
    def index(self):

        if request.method == 'POST':

            tick = request.form.get('tick')
            if not tick:
                return jsonify(code=10002, msg="IID Name cannot be empty")

            f = request.files['csv']
            if not f:
                return jsonify(code=10002, msg="Please upload csv file")
            tmpPath = '/tmp/%s' % (f.filename)
            f.save(tmpPath)
            csv_reader = csv.reader(open(tmpPath))
            isTitle = True

            total = succ = 0
            mysql = MySQL()
            for row in csv_reader:
                if isTitle:
                    if not self.checkTitle(row):
                        return jsonify(code=10001, msg="Error, title should be ['time', 'msec', 'price', 'volume', 'bid_price1', 'bid_volume1', 'ask_price1', 'ask_volume1']")
                    isTitle = False
                    continue

                total += 1
                if self.insertTickData(mysql, tick, row):
                    succ += 1                

            return jsonify(code=0, msg='ok', data={'total': total, 'succ': succ})

        return self.render('admin/tick_upload.html')


    def checkTitle(self, line):

        title = ['time', 'msec', 'price', 'volume', 'bid_price1', 'bid_volume1', 'ask_price1', 'ask_volume1']

        return line == title



    def insertTickData(self, mysql, iid, line):

        time = line[0]
        msec = line[1]
        price = line[2]
        volume = line[3]
        bidPrice1 = line[4]
        bidVolume1 = line[5]
        askPrice1 = line[6]
        askVolume1 = line[7]
        m2 = hashlib.md5()
        m2.update(iid + time + str(msec))
        token = m2.hexdigest()

        sql = '''
            INSERT INTO `tick_%s` (`token`, `iid`, `time`, `msec`, `price`, `volume`, `bid_price1`, `bid_volume1`, `ask_price1`, `ask_volume1`)
            VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')
        ''' % (filter(lambda x:x not in '0123456789', iid), token, iid, time, msec, price, volume, bidPrice1, bidVolume1, askPrice1, askVolume1)
        try:
            mysql.insert(sql)
        except Exception as e:
            return False

        return True



