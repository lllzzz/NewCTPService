from web.ctp import db

class Tick(db.Model):
    """docstring for Tick"""
    id = db.Column(db.Integer, primary_key=True)
    token = db.Column(db.String)
    iid = db.Column(db.String)
    time = db.Column(db.String)
    msec = db.Column(db.Integer)
    price = db.Column(db.Numeric)
    volume = db.Column(db.Integer)
    bid_price1 = db.Column(db.Numeric)
    bid_volume1 = db.Column(db.Integer)
    ask_price1 = db.Column(db.Numeric)
    ask_volume1 = db.Column(db.Integer)
    mtime = db.Column(db.DateTime)


class TickHc(Tick):
    __tablename__ = 'tick_hc'


