import serial
from flask import Flask
import subprocess
import os
import signal

pro=None

main_flag=True #1

app=Flask(__name__)
@app.route('/on')
def on():
    global pro
    pro=subprocess.Popen(["python patternrec.py"],stdout=subprocess.PIPE,shell=True)
    #patternrec.lol()
    return "ON"
@app.route('/off')
def off():
    global main_flag #1
    global pro
    main_flag=False #1
    pro.kill()
    #patternrec.loloff()
    return "OFF"
    '''try:
        os.killpg(os.getpgid(process.pid),signal.SIGTERM)
    except:
        pass'''

if __name__=='__main__':
    app.run(debug=True,host='0.0.0.0',threaded=True) #1
