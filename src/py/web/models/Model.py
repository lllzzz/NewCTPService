from web.ctp import db
from datetime import datetime

class Model(db.Model):
    """docstring for Tick"""
    __tablename__ = 'model'
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String, default='')
    nick_name = db.Column(db.String, default='')
    class_name = db.Column(db.String, default='')
    source = db.Column(db.Text, default='')
    status = db.Column(db.Integer, default=0)
    mtime = db.Column(db.DateTime, default=datetime.utcnow)
