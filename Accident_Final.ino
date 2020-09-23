/* FSR simple testing sketch.  <br>Connect one end of FSR to power, the other end to Analog 0.
Then connect one end of a 10K resistor from Analog 0 to ground 
*/
#include <SoftwareSerial.h>
#include <TinyGPS.h>

TinyGPS gps;

SoftwareSerial mySerial(9, 10);
SoftwareSerial mySerial1(4, 5);



int fsrPin = 0;     // the FSR and 10K pulldown are connected to a0
int fsrReading;     // the analog reading from the FSR resistor divider
const int buzzer = 12;
int i,pushbutton=2,flag,s;
 
void setup() {
  Serial.begin(9600); 
  mySerial.begin(9600);
  mySerial1.begin(9600);
  delay(100);  
  pinMode(fsrPin,INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(pushbutton,INPUT);
}
 
void loop() {
  flag=0;
  fsrReading=0;
  fsrReading = analogRead(fsrPin);  
  Serial.print("Analog reading = ");
  Serial.println(fsrReading);     // the raw analog reading
  /*if (fsrReading == 0) {
    Serial.println(" - No pressure");
  } else if (fsrReading < 10) {
    Serial.println(" - Light touch");
  } else if (fsrReading < 50) {
    Serial.println(" - Light squeeze");
  } else if (fsrReading < 150) {
    Serial.println(" - Medium squeeze");
  } */
  if(fsrReading==0)
   {
    Serial.println(" - Big squeeze");
        tone(buzzer,1000);
        for(int i=1;i<=100;i++)
        {
          Serial.println(digitalRead(pushbutton));
          if(digitalRead(pushbutton)==1)
          {
            flag=1; 
            break; 
          }
          delay(100);
        }  
       delay(1000);
     noTone(buzzer);
     if(flag==0)
     {
       Serial.println("Accident detected");
       Serial.println("Sending Sms");
       SendMessage();
       //noTone(buzzer);
       Serial.write(mySerial.read());
     }
     else
     Serial.println("No Accident");
  }
  delay(1000);
  //noTone(buzzer);
}
void SendMessage()
{
  //char c;
  String str="",str1="",lat="",lon="",lati1="",lati2="",loni1="",loni2="";
  while(mySerial1.available() > 0)
  {
     char c = mySerial1.read();
     str=str+c;
     //delay(100);
    Serial.print(c);
    if(gps.encode(c))
    {
      Serial.println("The end of the GPS sentence arrived. The data was parsed");
    }
    if(mySerial1.available()==0)
    {
      break;
    }
  } 
   int i,flag=0,c=0,j;
   for(i=0;i<str.length();i++)
   {
     if(str.charAt(i)=='A')
     flag=1;
     if(str.charAt(i)=='N' || str.charAt(i)=='E')
     c++;
     if(c==2)
     {
      str1=str1+str.charAt(i);
      break;
     }
     if(flag==1)
     {
       if(str.charAt(i)!='A' ||str.charAt(i)!=',')
       str1=str1+str.charAt(i);
     }
   }
   for(i=0;i<str1.length();i++)
   {
     if(str1.charAt(i)=='N')
     break;
     if(str1.charAt(i)!='A' && str1.charAt(i)!=',')
     lat = lat + str1.charAt(i);
   }
   for(j=i+1;j<str1.length();j++)
   {
     if(str1.charAt(j)=='E')
     break;
     if(str1.charAt(j)!=',')
     lon = lon + str1.charAt(j);
   }
   Serial.print(lat);
   Serial.print(" ");
   Serial.println(lon);
   int pos1=0,pos2=0;
   for(i=0;i<lat.length();i++)
   {
     if(lat.charAt(i)=='.')
     {
       pos1=i;
       break;
     }
   }
   for(i=0;i<lon.length();i++)
   {
      if(lon.charAt(i)=='.')
      {
        pos2=i;
        break;
      }
   }
   float la1=0,la2=0,lo1=0,lo2=0;
   for(i=0;i<lat.length();i++)
   {
     if(i<(pos1-2))
     lati1=lati1+lat.charAt(i);
     else
     lati2=lati2+lat.charAt(i);
   }
   for(i=0;i<lon.length();i++)
   {
     if(i<(pos2-2))
     loni1=loni1+lon.charAt(i);
     else
     loni2=loni2+lon.charAt(i);
   }
   Serial.print(lati1);
   Serial.print(" ");
   Serial.print(lati2);
   Serial.print(" ");
   Serial.print(loni1);
   Serial.print(" ");
   Serial.println(loni2);
   c=1;
   for(i=lati1.length()-1;i>=0;i--)
   {
     la1=(lati1.charAt(i)-48)*c+la1;
     c=c*10;
   }
   int c1=pos1,f=10;
   for(i=0;i<lati2.length();i++)
   {
    if(lati2.charAt(i)=='.')
    {
      pos1=i;
      break;
    }
   }
   for(i=0;i<pos1;i++)
   {
     la2 = la2 + (lati2.charAt(i)-48);
     if(i<pos1-1)
     la2=la2*10;
   }
   for(i=pos1+1;i<lati2.length();i++)
   {
       la2 = la2 + (((float)(lati2.charAt(i)-48))/f);
       f=f*10;
   }
   f=10;
   c=1;
   for(i=loni1.length()-1;i>=0;i--)
   {
     lo1=(loni1.charAt(i)-48)*c+lo1;
     c=c*10;
   }
   for(i=0;i<loni2.length();i++)
   {
    if(loni2.charAt(i)=='.')
    {
      pos2=i;
      break;
    }
   }
   for(i=0;i<pos2;i++)
   {
     lo2 = lo2 + (loni2.charAt(i)-48);
     if(i<pos2-1)
     lo2=lo2*10;
   }
   for(i=pos2+1;i<loni2.length();i++)
   {
       lo2 = lo2 + (((float)(loni2.charAt(i)-48)))/f;
       f=f*10;
   }
   Serial.print(la1);
   Serial.print(" ");
   Serial.println(la2);
   Serial.print(" ");
   Serial.print(lo1);
   Serial.print(" ");
   Serial.println(lo2);
   la1=la1+(la2/60);
   lo1=lo1+(lo2/60);
   Serial.print(la1);
   Serial.print(" ");
   Serial.println(lo1);
   mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+918789774814\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.print("Accident Detected at location");
  mySerial.print("http://maps.google.com/maps?q=");// The SMS text you want to send
  mySerial.print(la1); 
  mySerial.print(",");
  mySerial.println(lo1);
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  //noTone(buzzer);
} 
