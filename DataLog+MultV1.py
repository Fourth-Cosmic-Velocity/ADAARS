import threading
import time
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
        
        print(i*3)
        print('\n')
        time.sleep(0.2)

def T2():
    for j in range (16):
        
        print(j*2)
        print('\n')
        time.sleep(0.2)



T1 = threading.Thread(target=T1)
T2 = threading.Thread(target=T2)

T1.start()
T2.start()

if j==16:
        print ('Program stopped')
        running = False
        file.close()
