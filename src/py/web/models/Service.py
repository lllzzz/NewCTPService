from web.ctp import db
from datetime import datetime

class Service(db.Model):
    """docstring for Tick"""
    __tablename__ = 'service'
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String, default='')
    cmd = db.Column(db.String, default='')
    run_config = db.Column(db.String, default='')
    mtime = db.Column(db.DateTime, default=datetime.utcnow)
