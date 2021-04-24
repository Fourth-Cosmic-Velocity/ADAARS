import time
#create a variable to control the while loop
running = True
A = 0
B = 1
C = 2
#new .txt file created with header
file = open('sensor_readings.csv', 'a')
#file.write(time.strftime('%H:%M:%S %d/%m/%Y') + ', ' + str(temperature) + ', '+ str(temperature_f)+',' + str(humidity) + '\n')
file.write('Date'+ ',' + 'Time' + ',' +'A' + ', '+ 'B' +',' + 'C' + '\n')
time.sleep(1)
#loop forever
for i in range (10):
        
        GCST= int(round(time.time() * 1000))
        GCET = GCST + 1000
        while time.time()*1000 < GCET:

            #try:
                #while True:
                A=A+1
                B=B+B
                C=C*2
                print(A,B,C)
                file.write(time.strftime('%d/%m/%Y')+ ',' + time.strftime('%H:%M:%S') + ',' + str(A) + ', '+ str(B) +',' + str(C) + '\n')
                #file.write(time.strftime('%H:%M:%S %d/%m/%Y'))
                time.sleep(0.00001)
        file.write('BREAK'+str(i)+'\n')

            #except KeyboardInterrupt:
print ('Program stopped')
running = False
file.close()
