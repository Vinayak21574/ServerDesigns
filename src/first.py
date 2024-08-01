from copy import deepcopy
import pandas as pd
import numpy as np
from pydantic import BaseModel
import matplotlib.pyplot as plt

id="3000"

class flow(BaseModel):
    start:float=-1
    end:float=-1
    sent:int=0
    recv:int=0
    fin:bool=False
    prev_lat:float=-1
    latency:float=-1

def pacType(here):
    time=here[0]
    src=here[1]
    dest=here[2]
    size=here[3]

    l=here[4].split()
    Psrc=l[0]
    Pdest=l[2]
    fin=False
    syn=False
    ack=False
    if 'FIN' in l[3]:
        fin=True
    if 'SYN' in l[3]:
        syn=True
    if 'ACK' in l[3]:
        ack=True

    if(src=="192.168.0.4"):
        client=(dest,Pdest)
        server=(src,Psrc)
        sendMode=True
    else:
        server=(dest,Pdest)
        client=(src,Psrc)
        sendMode=False

    return (server,client,time,syn,fin,ack,size,sendMode)

FLOW=[]
curr={}


data=pd.read_csv(f'./{id}.csv')
data=data[data['Protocol']=="TCP"]
data.drop('Protocol',axis=1,inplace=True)
data.drop('No.',axis=1,inplace=True)
data=data.to_numpy()


rows,cols=data.shape

for i in range(rows):
    server,client,time,syn,fin,ack,size,sendMode=pacType(data[i])
    if (client[1] in curr.keys()):
        if(syn and curr[client[1]].fin):
            FLOW.append(deepcopy(curr[client[1]]))
            curr[client[1]]=flow(start=time,received=size)

        if(ack and not(fin) and not(syn) and curr[client[1]].fin):
            curr[client[1]].latency+=time
            curr[client[1]].end=time

        if(sendMode):
            curr[client[1]].sent+=size
        else:
            curr[client[1]].recv+=size
        
        if(fin):
            curr[client[1]].fin=True
            curr[client[1]].end=time
            if(curr[client[1]].prev_lat==-1 and curr[client[1]].latency==-1):
                curr[client[1]].prev_lat=time
            elif(curr[client[1]].prev_lat==-1 and curr[client[1]].latency!=-1):
                curr[client[1]].latency=-time

    else:
        curr[client[1]]=flow(start=float(time),recv=int(size))

for entry in curr.keys():
    FLOW.append(deepcopy(curr[entry]))


clients=[]
send_tp=[]
recv_tp=[]
latency=[]

count=1

for entry in FLOW:
    if(entry.fin and (entry.start<=entry.end)):
        temp=entry.end-entry.start
        latency.append(1000*temp)
        clients.append(count)
        count+=1
        send_tp.append(8*entry.sent/(50*temp))
        recv_tp.append(8*entry.recv/(50*temp))


plt.scatter(x=clients,y=send_tp,c="red")
plt.scatter(x=clients,y=recv_tp,c="blue")

plt.legend(("SendTP","RecvTP"),scatterpoints=1)

plt.xlabel("Clients")
plt.ylabel("Throughput(bits/sec)")

plt.savefig(f'{id}_Throughput.png')
plt.show()

plt.scatter(x=clients,y=latency,c="coral")

plt.xlabel("Clients")
plt.ylabel("Latency(msec)")
plt.savefig(f'{id}_Latency.png')
plt.show()
