import time
import threading
i=0
j=0
k=0
l=0
#create a variable to control the while loop
running = True
#new .txt file created with header
file = open('sensor_readings.csv', 'a')
#file.write(time.strftime('%H:%M:%S %d/%m/%Y') + ', ' + str(temperature) + ', '+ str(temperature_f)+',' + str(humidity) + '\n')
file.write('Date'+ ',' + 'Time' + ',' + 'A' + ', '+ 'B' +',' + 'C' + ',' +'D' + '\n')
file.write('\n')
time.sleep(1)
def T1():
        
    for i in range (16):
        i=i+1
        time.sleep(2)
                
        file.write(time.strftime('%d/%m/%Y')+ ',' + time.strftime('%H:%M:%S') + ',' + str(i) + ', '+ str(j) +',' + str(k) + ',' + str(l)+'\n')

def T2():
    
    for j in range (16):
        j=(j+1)*(j+2)
        time.sleep(2)
        print(j)        
        file.write(time.strftime('%d/%m/%Y')+ ',' + time.strftime('%H:%M:%S') + ',' + str(i) + ', '+ str(j) +',' + str(k) + ',' + str(l)+'\n')

    T1 = threading.Thread(target=T1)
    T2 = threading.Thread(target=T2)
    
    T1.start()
    T2.start()
    
#loop forever

if j=='120':
        print ('Program stopped')
        running = False
        file.close()
