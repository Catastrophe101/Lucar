import serial
from flask import Flask
import subprocess
import os
import signal
import psutil


pro=None

app=Flask(__name__)
@app.route('/on')
def on():
    global pro
    pro=subprocess.Popen(["python patternrec.py"],stdout=subprocess.PIPE,shell=True)
    #patternrec.lol()
    return "ON"
@app.route('/off')
def off():
    global pro
    parent=psutil.Process(pro.pid)
    children=parent.children(recursive=True)
    for child in children:
        child.kill()
    parent.kill()
    parent.wait(5)
    #pro.kill()
    #patternrec.loloff()
    #killstr="kill -9"+str(pro.pid)
    #print killstr
    #prokill=subprocess.Popen([killstr],stdout=subprocess.PIPE,shell=False)
    return "OFF"
    '''try:
        os.killpg(os.getpgid(process.pid),signal.SIGTERM)
    except:
        pass'''

if __name__=='__main__':
    app.run(debug=True,host='0.0.0.0',threaded=True) #1
