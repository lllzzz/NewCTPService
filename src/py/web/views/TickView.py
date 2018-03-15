from flask_admin import BaseView,expose
from flask_admin.contrib.sqla import ModelView

class TickView(ModelView):
    can_delete = False
    can_create = False
    can_edit = False
    can_view_details = True

