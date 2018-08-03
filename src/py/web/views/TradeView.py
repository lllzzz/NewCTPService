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

class TradeView(ModelView):
    can_delete = False
    can_create = False
    can_edit = False
    can_view_details = True
    column_default_sort = ('send_time', True)
