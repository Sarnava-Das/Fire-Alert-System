import serial
from twilio.rest import Client
import sys

import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart

def send_sms(data):
    try:
        # Twilio credentials
        twilio_account_sid = 'twilio_account_sid'
        twilio_auth_token = ' twilio_auth_token'
        twilio_phone_number = 'twilio_phone_number'
        recipient_phone_number = 'recipient_phone_number'

        # Twilio client
        twilio_client = Client(twilio_account_sid, twilio_auth_token)

        twilio_client.messages.create(
                            body="Office maybe on fire",
                            from_=twilio_phone_number,
                            to=recipient_phone_number)
       
    except Exception as e:
        print(f"An error occurred: {str(e)}")

def read_data():
    # Arduino serial connection
    ser = serial.Serial('COM3', 9600)  
    
    try:
      
        while True:
            # Read data from the Arduino
            arduino_data = ser.readline().decode().strip()
            
            
            # Split the line based on the ':' separator
            parts = arduino_data.split(':')
            if len(parts) == 2:
                sensor_type, sensor_value = parts[0], parts[1]
            
            if sensor_type == 'Smoke' and sensor_value.isdigit():
                     if int(sensor_value) >100:
                         send_sms(sensor_value)
                         send_email("Smoke Fire Alert",sensor_value,"There maybe be fire in the building...")
                        
                
            if sensor_type == 'Flame' and sensor_value.isdigit():
                     if int(sensor_value) < 100:
                         send_sms(sensor_value)
                         send_email("Flame Fire Alert",sensor_value,"There maybe be fire in the building...")
                        
            
    except KeyboardInterrupt:
        print("Exiting the program")
 
    finally:
         ser.close() 
        
   

# Define the email sender function
def send_email(sensor_subject,data,body):
    try:
        # Email configuration
        sender_email = "sender_mail"
        app_password = "app password"  # Use the App Password generated
        receiver_email = "receiver_mail"
        subject = sensor_subject
        message = f"{body} {data}"

        # Create the email message
        msg = MIMEMultipart()
        msg['From'] = sender_email
        msg['To'] = receiver_email
        msg['Subject'] = subject

        msg.attach(MIMEText(message, 'plain'))

        server = smtplib.SMTP('smtp.gmail.com', 587)
        server.starttls()

        # Log in to your email account using the App Password
        server.login(sender_email, app_password)

        # Send the email
        server.sendmail(sender_email, receiver_email, msg.as_string())
        
    except smtplib.SMTPAuthenticationError as e:
            print(f"SMTP Authentication Error: {e}")
    except Exception as e:
            print(f"An error occurred: {e}")
    finally:
        # Quit the SMTP server
        server.quit()

      
def main():
    read_data()

if __name__ == "__main__":
    main()

              