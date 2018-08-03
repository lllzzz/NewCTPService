from web.ctp import db
from datetime import datetime

class Trade(db.Model):
    """docstring for Tick"""
    __tablename__ = 'trade'
    id = db.Column(db.Integer, primary_key=True)
    # source = db.Column(db.String, default='', name='from')
    iid = db.Column(db.String, default='')
    trade_id = db.Column(db.String, default='')
    action = db.Column(db.String, default='')
    ctp_front_id = db.Column(db.Integer, default='')
    ctp_session_id = db.Column(db.Integer, default='')
    ctp_exchange_id = db.Column(db.String, default='')
    ctp_sys_order_id = db.Column(db.String, default='')
    ctp_td_req_id = db.Column(db.Integer, default='')
    price = db.Column(db.Numeric, default='')
    volume = db.Column(db.Integer, default='')
    is_buy = db.Column(db.Integer, default='')
    is_open = db.Column(db.Integer, default='')
    is_today = db.Column(db.Integer, default='')
    send_time = db.Column(db.String, default=0)
    deal_price = db.Column(db.Numeric, default=0)
    deal_volume = db.Column(db.Integer, default=0)
    status = db.Column(db.Integer, default=0)
    mtime = db.Column(db.DateTime, default=datetime.utcnow)
