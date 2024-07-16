#!/use/bin/env python3

#########################################################################################
# Copyright (C) 2022 Mono Wireless Inc. All Rights Reserved.
# Released under MW-OSSLA-1J,1E (MONO WIRELESS OPEN SOURCE SOFTWARE LICENSE AGREEMENT).
#########################################################################################

#########################################################################################
# IMPORTS
#########################################################################################
# generic modules
import sys
import os
from datetime import datetime
import base64
from io import BytesIO

# sqlite3 (Lightweight SQL database)
import sqlite3

# Flask (Lighweight HTTP server)
from flask import Flask,send_from_directory,render_template,request,g

# matplotlib (Graphing)
from matplotlib.figure import Figure
import matplotlib.dates as mdates

#########################################################################################
# VARIABLES
#########################################################################################

# get dirname, filename of this script
scr_dir, scr_fname = os.path.split(os.path.abspath(__file__))

# app object of Flask
app = Flask(__name__)

### configs (config.ini)
# example:
#     [SQLITE3]
#     db_file = ../log/your_db_file.sqlite
import configparser
config = configparser.ConfigParser()
config.read(os.path.join(scr_dir, 'config.ini'))
try: conf_db_filename = os.path.abspath(os.path.join(scr_dir, config['SQLITE3']['db_file']))
except: conf_db_filename = os.path.abspath(os.path.join(scr_dir, '../log/TWELITE_Stage_WSns.sqlite'))

#########################################################################################
# DICTIONARIES
#########################################################################################

# labels by paket ID
# OR packet ID by string label.
dict_pkt_type = {
    1 : ('PAL MAG', 'MAG', 'N/A', 'N/A', 'N/A'),
    2 : ('PAL AMB', '温度[℃]', '湿度[%]', '照度[lx]', 'N/A'),
    3 : ('PAL MOT','X[G]', 'Y[G]', 'Z[G]', 'N/A'),
    5 : ('CUE','X[G]', 'Y[G]', 'Z[G]', 'N/A'),
    6 : ('ARIA','温度[℃]', '湿度[%]', 'N/A', 'N/A'),
    257 : ('App_TWELITE','DI0/1/2/3', 'AD1[V]', 'AD2[V]', 'AD3[V]'),
    'ARIA' : 6,
    'PAL_AMB' : 2,
    'PAL_MOT' : 3,
    'CUE' : 5,
    'MAG' : 1,
    'APPTWELITE' : 257
}

# labels for graph plotting (must not include non-ascii chars)
dict_label = {
    1 : ('PAL MAG', 'MAG', 'N/A', 'N/A', 'N/A'),
    2 : ('PAL AMB', 'TEMP[C]', 'HUMID[%]', 'LUMI[lx]', 'N/A'),
    3 : ('PAL MOT','X[G]', 'Y[G]', 'Z[G]', 'N/A'),
    5 : ('CUE','X[G]', 'Y[G]', 'Z[G]', 'N/A'),
    6 : ('ARIA','TEMP[C]', 'HUMID[%]', 'N/A', 'N/A'),
    257 : ('App_TWELITE','DI1/2/3/4', 'AD1[V]', 'AD2[V]', 'AD3[V]'),
}

# Label string corresponding to MAG data
dict_mag = {
    0 : 'なし',
    1 : 'N極',
    2 : 'S極',
    3 :  None
}

# Label string corresponding to DIO bitmap data.
#   configure the table to display H/L in the order of D1/D2/D3/D4
dict_dio = {
    0 : 'H/H/H/H',
    1 : 'L/H/H/H',
    2 : 'H/L/H/H',
    3 : 'L/L/H/H',
    4 : 'H/H/L/H',
    5 : 'L/H/L/H',
    6 : 'H/L/L/H',
    7 : 'L/L/L/H',
    8 : 'H/H/H/L',
    9 : 'L/H/H/L',
    10: 'H/L/H/L',
    11: 'L/L/H/L',
    12: 'H/H/L/L',
    13: 'L/H/L/L',
    14: 'H/L/L/L',
    15: 'L/L/L/L',
}

# Label string corresponding to CUE event data.
dict_ev_cue = {
    1 : '面１', # 1 of the dice face 
    2 : '面２',
    3 : '面３',
    4 : '面４',
    5 : '面５',
    6 : '面６',
    8 : 'シェイク', # shaking
    16 : 'ムーブ', # moving
}

#########################################################################################
# SUB ROUTINES
#########################################################################################

# convert integer value into hex value (assuming int32_t)
def tohex_i32(val):
    return hex((val + (1 << 32)) % (1 << 32))[2:].upper()

# convert hex value into integer value (assuming int32_t)
def toint_i32(val):
    x = int(val, 16)
    if x >= 2**31:
        x -= 2**32
    return x

# plot a fig.
# @ax       subplot object.
# @v_time   list of X-AXIS (time)
# @v_data   list of Y-AXIS (data)
# @lbl      label of data
# @col      color of plotting line
# @returns   NONE
def _graph_plot(ax, v_time, v_data, lbl, col):
        ax.tick_params(labelsize = 6.5) 
        ax.plot(v_time, v_data, label=lbl, color=col)
        #ax.set_ylabel(lbl)
        ax.legend(loc='upper left')
        ax.xaxis.set_major_formatter(mdates.DateFormatter("%H:%M"))

# output fig as <img...> tag with embedded data.
# @fig      figure object which is already plotted.
# @returns  image tag data with data.
def _graph_get_img_tab_embedded_data(fig):
    buf = BytesIO()
    fig.savefig(buf, format="png")
    # Embed the result in the html output.
    data = base64.b64encode(buf.getbuffer()).decode("ascii")
    return f"<img src='data:image/png;base64,{data}'/>"

# get description from DB by SID
# @param cur        DB cursor
# @param i32sid     Integer of SID in int32_t.
# @returns          Desc string.
def get_desc(cur, i32sid):
    desc = ''
    try:
        cur.execute('''SELECT * FROM sensor_node WHERE (sid = ?)''', (i32sid,))
        d = cur.fetchone()
        desc = d[2]
    except:
        pass

    return desc

# get latest timestamp from DB by SID
# @param cur        DB cursor
# @param i32sid     Integer of SID in int32_t.
# @returns          latest timestamp in integer, 0 when error.
def get_latest_ts(cur, i32sid):
    latest = 0
    try:
        cur.execute('''SELECT * FROM sensor_last WHERE (sid = ?)''', (i32sid,))
        d = cur.fetchone()
        latest = int(d[1])
    except:
        pass

    return latest


#########################################################################################
# Flask related functions
#########################################################################################

# open the db in `g`, global for session
def db_open():
    if 'db' not in g:
        g.db = sqlite3.connect(conf_db_filename)
        #app.logger.debug("opening DB %s" % g.db)

    return g.db

# called when a session is over, closing the db.
@app.teardown_appcontext
def teardown_db(exception):
    db = g.pop('db', None)

    if db is not None:
        #app.logger.debug("closing DB %s" % db)
        db.close()
        
### Handler for address '/'
# Enumerate SIDs where sensor data exists.
@app.route('/')
def index():
    global dict_pkt_type
    # query result
    r = []
    result = []

    # open data base
    cur = db_open().cursor()

    # find SIDs
    cur.execute('''SELECT * FROM sensor_last ORDER BY ts DESC''')
    # find Desc for each SID
    for sid, ts, lid, lqi, pkt_type, value, value1, value2, value3, val_vcc_mv, val_dio, ev_id in cur.fetchall():
        cur.execute('''SELECT * FROM sensor_node WHERE (sid = ?)''', (sid,))
        d = cur.fetchone()

        # PKT TYPE
        lblinfo = ('UNK',)
        if pkt_type in dict_pkt_type: lblinfo=dict_pkt_type[pkt_type]

        # CUE EVENT
        if pkt_type == dict_pkt_type['CUE']:
            if ev_id in dict_ev_cue: 
                ev_id = dict_ev_cue[ev_id]

        r.append((sid, d[1], d[2], ts, datetime.fromtimestamp(ts) # SID(int32), SID(TEXT), DESC(TEXT), ts(EPOCH)
                , (lblinfo[0], lid, lqi, value, value1, value2, value3, val_vcc_mv, val_dio, ev_id))) 
            
    
    # sort by ID
    result = sorted(r, key=lambda x : x[1])

    # returns
    return render_template('index.html', data = result)

### Handler for POST request of the address '/year'.
# Enumerate the years for which sensor data exist from the specified SID.
# the main proc
def _list_years(sid, i32sid):
    # open data base and query
    cur = db_open().cursor()
    cur.execute('''SELECT DISTINCT year FROM sensor_data WHERE sid = ? ORDER BY year ASC''', (i32sid,))
    result = cur.fetchall()
    desc = get_desc(cur, i32sid)
    return render_template('year.html', sid = sid, i32sid = i32sid, desc = desc, data = result)

### Handler for POST request of the address '/month'.
# Enumerates the months in which data exist from the specified SID and year
def _list_months(sid, i32sid, year):
    # open data base and query
    cur = db_open().cursor()
    cur.execute('''SELECT DISTINCT month FROM sensor_data WHERE (sid=?) AND (year=?) ORDER BY month ASC''', (i32sid,year,))
    result = cur.fetchall()
    desc = get_desc(cur, i32sid)
    return render_template('month.html', sid = sid, i32sid = i32sid, desc = desc, year = year, data = result)

### Handler for POST request of the address '/day'
# Enumerates the days for which data exist from the specified SID, year, and month.
def _list_days(sid, i32sid, year, month):
    # open data base and query
    cur = db_open().cursor()
    cur.execute('''SELECT DISTINCT day FROM sensor_data 
                   WHERE (sid=?) AND (year=?) AND (month=?) ORDER BY month ASC''', (i32sid,year,month,))
    result = cur.fetchall()
    desc = get_desc(cur, i32sid)
    return render_template('day.html', sid = sid, i32sid = i32sid, desc = desc, year = year, month=month, data = result)

### Handler for POST request of the address '/show_the_day'.
# Enumerate the list of sensor data present for the specified SID, year, month, and day.
def _show_the_day(sid, i32sid, year, month, day):
    global dict_pkt_type, dict_mag, dict_dio, dict_ev_cue

    # open data base and query
    cur = db_open().cursor()
    desc = get_desc(cur, i32sid)

    cur.execute('''SELECT ts,lid,lqi,pkt_type,value,value1,value2,value3,val_vcc_mv,val_dio,ev_id FROM sensor_data
                   WHERE (sid=?) and (year=?) and (month=?) and (day=?)
                   ''', (i32sid,year,month,day,))
    r = cur.fetchall()
    result =[]

    # check first sample (determine packet type, etc)
    lblinfo = ('UNKNOWN', 'VAL', 'VAL1', 'VAL2', 'VAL3')
    pkt_type = None
    lid = None
    if len(r) > 0:
        try:
            # pick first sample
            r0 = r[0]
            # packet type
            pkt_type = int(r0[3])
            if pkt_type in dict_pkt_type: lblinfo=dict_pkt_type[pkt_type]
            # logical ID (normally, all the same)
            lid = r0[1]
        except:
            pass

    ct = 0
    for ts,lid,lqi,pkt_type,value,value1,value2,value3,val_vcc_mv,val_dio,ev_id in r:
        try:
            lt = datetime.fromtimestamp(ts) # convert unix epoch timestamp to local time object.

            # mag info
            mag = None
            if val_dio & 0x10000000:
                mag = dict_mag[(val_dio >> 24) & 0x3]

            if pkt_type == 1: value = mag

            # DIO
            if pkt_type == dict_pkt_type['APPTWELITE']:
                bm_dio = int(value)
                if bm_dio >= 0 and bm_dio <= 15:
                    value = dict_dio[bm_dio]

            # EVENT ON CUE
            if pkt_type == dict_pkt_type['CUE']:
                if ev_id in dict_ev_cue: 
                    ev_id = dict_ev_cue[ev_id]

        except:
            pass

        result.append((ct,lt,lqi,value,value1,value2,value3,val_vcc_mv,mag,ev_id))
        ct = ct + 1

    return render_template('show_the_day.html', 
                sid = sid, i32sid = i32sid, desc = desc, year = year, 
                month=month, day=day, data = result, lid=lid, lblinfo=lblinfo)


# grenrate graph data.
# @param latest_ts              if not 0, render graph from latest_ts - 1day to latest_ts.
# @param yera, month, day       specify YYYY/MM/DD (where latest_ts==0)
def _graph_a_day(sid, i32sid, latest_ts, year, month, day):
    global dict_label

    # open data base and query
    cur = db_open().cursor()
    if latest_ts == 0:
        cur.execute('''SELECT ts,lid,lqi,pkt_type,value,value1,value2,value3,val_vcc_mv,val_dio,ev_id FROM sensor_data
                    WHERE (sid=?) and (year=?) and (month=?) and (day=?)
                    ORDER BY random() LIMIT 1024''', (i32sid,year,month,day,))
    else:
        cur.execute('''SELECT ts,lid,lqi,pkt_type,value,value1,value2,value3,val_vcc_mv,val_dio,ev_id FROM sensor_data
                    WHERE (sid=?) and (ts BETWEEN ? and ?)
                    ORDER BY random() LIMIT 1024''', (i32sid,latest_ts-86399,latest_ts,))
        lt = datetime.fromtimestamp(latest_ts) # convert unix epoch timestamp to local time object.
        year = lt.year
        month = lt.month
        day = lt.day
        
    r = cur.fetchall()

    # check first sample (determine packet type, etc)
    lblinfo = ('UNKNOWN', 'VAL', 'VAL1', 'VAL2', 'VAL3')
    pkt_type = None
    lid = None
    if len(r) > 0:
        try:
            # pick first sample
            r0 = r[0]
            # packet type
            pkt_type = int(r0[3])
            if pkt_type in dict_label: lblinfo=dict_label[pkt_type]
            # logical ID (normally, all the same)
            lid = r0[1]
        except:
            pass

    # list for plotting.
    v_t = [] # X-AXIS (time)
    v_0 = []
    v_1 = []
    v_2 = []
    
    # sorting the list (use random pick during SQL query, but sorted by ts is better for grapphing)
    sr = sorted(r, key=lambda x : x[0])
    ct = 1
    for ts,lid,lqi,pkt_type,value,value1,value2,value3,val_vcc_mv,val_dio,ev_id in sr:
        lt = datetime.fromtimestamp(ts) # convert unix epoch timestamp to local time object.
        v_0.append(value)
        v_1.append(value1)
        v_2.append(value2)
        v_t.append(lt)

    ### plotting figs.
    fig = Figure()
    fig.set_size_inches(5, 10)
    fig.suptitle("%s - %04d/%02d/%02d - %s" % (sid, int(year), int(month), int(day), lblinfo[0]))
    
    # first fig
    _graph_plot(fig.add_subplot(3, 1, 1), v_t, v_0, lblinfo[1], 'red')

    # second fig
    if v_1.count(None) < len(v_1):
        _graph_plot(fig.add_subplot(3, 1, 2), v_t, v_1, lblinfo[2], 'green')
        
    # 3rd fig
    if v_2.count(None) < len(v_2):
        _graph_plot(fig.add_subplot(3, 1, 3), v_t, v_2, lblinfo[3], 'blue')

    # set layout strategy   
    fig.tight_layout()  

    return _graph_get_img_tab_embedded_data(fig)

### Handlers for /{SID}/...

# /<sid> should be hex 8digit string, but some others might be captured (like favicon.ico).
# if the request is other than SID, forward request to the file at `static/root/`.
@app.route('/<sid>')
def list_years_url(sid):
    app.logger.debug(sid)
    try:
        i32sid = toint_i32(sid)
    except:
        app.logger.debug("/{STRING}=%s" % sid)
        i32sid = None

    if i32sid is None: return send_from_directory(os.path.join(app.root_path, 'static/root/'), sid, )
    else: return _list_years(sid, i32sid)

# query months by SID and YEARs
@app.route('/<string:sid>/<int:year>')
def list_months_url(sid, year):
    i32sid = toint_i32(sid)
    return _list_months(sid, i32sid, year)

# query days by SID, YEAR, MONTH
@app.route('/<string:sid>/<int:year>/<int:month>')
def list_days_url(sid, year, month):
    i32sid = toint_i32(sid)
    return _list_days(sid, i32sid, year, month)

# show sensor data list queied by SID, YEAR, DAY.
@app.route('/<string:sid>/<int:year>/<int:month>/<int:day>/l')
def show_the_day_url(sid, year, month, day):
    i32sid = toint_i32(sid)
    return _show_the_day(sid, i32sid, year, month, day)

# plot sensor data queied by SID, YEAR, DAY.
@app.route('/<string:sid>/<int:year>/<int:month>/<int:day>/g')
def graph_the_day_url(sid, year, month, day):
    i32sid = toint_i32(sid)
    return _graph_a_day(sid, i32sid, 0, year, month, day)

# plot latest sensor data.
@app.route('/<string:sid>/g')
def graph_the_latest_url(sid):
    i32sid = toint_i32(sid)
    latest_ts = get_latest_ts(db_open().cursor(), i32sid)
    app.logger.debug("TS=%d" % latest_ts)
    return _graph_a_day(sid, i32sid, int(latest_ts), 0, 0, 0)

#########################################################################################
# RUN THE APP
#########################################################################################
if __name__ == '__main__':
    app.debug = True # debug option
    app.run(host='localhost') # local machine only.
    # app.run(host='0.0.0.0') # exporting to LAN.
