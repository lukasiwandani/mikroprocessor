/*
 *  This sketch demonstrates how to scan WiFi networks. 
 *  The API is almost the same as with the WiFi Shield library, 
 *  the most obvious difference being the different file you need to include:
 */
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266TelegramBOT.h>

#define botName "blue"
#define botUserName "blueassistant_bot"
#define botToken "481532304:AAGY62jkVkiEGiXCOWiFk4H7kWqN_WnLAAo" //replace with your bot token
#define adminID "390065272" //your ID

static char SSID[] = "Wifi tidak tersedia";
static char password[] =  "gakadapaswodnya";
int qtyA;
int qtyB;
int qtyC;

TelegramBOT bot(botToken, botName, botUserName);

void setup() 
{
	pinMode(16,OUTPUT);
	pinMode(5,OUTPUT);
	pinMode(4,OUTPUT);

	digitalWrite(16,LOW);
	digitalWrite(5,LOW);
	digitalWrite(4,LOW);
	delay(200);
	digitalWrite(16,HIGH);
	digitalWrite(5,HIGH);
	digitalWrite(4,HIGH);
	
    Serial.begin(115200);
	Serial.println("Connecting to " + String(SSID) + " ");
    WiFi.begin(SSID, password);
    // Wait until it is connected to the network
	while (WiFi.status() != WL_CONNECTED) 
	{
	    delay(500);
	    Serial.print(".");
	}
	Serial.println("\n\nWiFi connected.");  
	Serial.print("  IP address: " + String(WiFi.localIP()) + "\n");
	delay(3000);
	// Start the bot and send a message to admin, so we know that ESP8266 has booted.
	bot.begin();
    bot.sendMessage(adminID, "I am online now!", "");
	bot.sendMessage(adminID, "Please enter /info ", "");
}

void loop() 
{

	int itemselected = 0;
	// React on an incomming message:
	/* 	
		GENERAL
			bot.message[0][0] ... number of received messages
			bot.message[0][1] ... code of last message
		MESSAGE
			bot.message[i][0] ... update ID
			bot.message[i][1] ... user ID
			bot.message[i][2] ... first name
			bot.message[i][3] ... last name
			bot.message[i][4] ... user ID
			bot.message[i][5] ... message
	*/
	bot.getUpdates(bot.message[0][1]);
	for (int i = 1; i < bot.message[0][0].toInt() + 1; i++)
	{
		// Only the admin is allowed to talk to the bot
		if (bot.message[i][4] == adminID)
		{
			// extract the message
			bot.message[i][5] = bot.message[i][5].substring(1, bot.message[i][5].length());
			if (bot.message[i][5] == "info")
			{
				bot.sendMessage(bot.message[i][4], "Hello, I am your Bot", "");
				bot.sendMessage(bot.message[i][4], "Please type /help for commands", "");
            }
            else if ((bot.message[i][5] == "help"))
			{
                bot.sendMessage(bot.message[i][4], " /info - get info about this bot" , "");
				bot.sendMessage(bot.message[i][4], " /help - list all available commands" , "");
				bot.sendMessage(bot.message[i][4], " /list - list item for sale" , "");
				bot.sendMessage(bot.message[i][4], " /select_(item code)_qty - select item that you want to purchase example /select_001_2 if you want to buy itemA for 2 pcs" , "");
				bot.sendMessage(bot.message[i][4], " /done - proceed to payment and get your item" , "");				
				bot.sendMessage(bot.message[i][4], " /getuptime - get the system uptime" , "");
			}
			else if ((bot.message[i][5] == "list"))
			{
				bot.sendMessage(bot.message[i][4], "Code : 001 | Name : itemA | Unit Price : 5000", "");
				bot.sendMessage(bot.message[i][4], "Code : 002 | Name : itemB | Unit Price : 3000", "");
				bot.sendMessage(bot.message[i][4], "Code : 003 | Name : itemC | Unit Price : 8000", "");
			}
			else if ((bot.message[i][5].substring(0,6) == "select"))
			{
				if ((bot.message[i][5].substring(7,10)) == "001"){
						qtyA = qtyA + bot.message[i][5].substring(11).toInt();
						bot.sendMessage(bot.message[i][4], "itemA = " + String(qtyA), "");	
					
				}
				else if ((bot.message[i][5].substring(7,10)) == "002"){
						qtyB = qtyB + bot.message[i][5].substring(11).toInt();
						bot.sendMessage(bot.message[i][4], "itemB = " + String(qtyB), "");				
				}
				else if ((bot.message[i][5].substring(7,10)) == "003"){
						qtyC = qtyC + bot.message[i][5].substring(11).toInt();
						bot.sendMessage(bot.message[i][4], "itemC = " + String(qtyC), "");	
				}
				else {
						bot.sendMessage(bot.message[i][4], "wrong item code", "");
				}
			}
			else if ((bot.message[i][5] == "done"))
			{
					bot.sendMessage(bot.message[i][4], "itemA = " + String(qtyA) + "pcs * 5000 = " + String(qtyA * 5000), "");
					bot.sendMessage(bot.message[i][4], "itemB = " + String(qtyB) + "pcs * 3000 = " + String(qtyB * 3000), "");
					bot.sendMessage(bot.message[i][4], "itemC = " + String(qtyC) + "pcs * 8000 = " + String(qtyC * 8000), "");
					bot.sendMessage(bot.message[i][4], "Total payment: " + String(qtyA * 5000 + qtyB * 3000 + qtyC * 8000), "");
					bot.sendMessage(bot.message[i][4], "tunggu sampai barang anda keluar semua", "");
					
					digitalWrite(16,LOW);
					delay(qtyA * 1000);
					digitalWrite(16,HIGH);
					digitalWrite(5,LOW);
					delay(qtyB * 1000);
					digitalWrite(5,HIGH);
					digitalWrite(4,LOW);
					delay(qtyC * 1000);
					digitalWrite(4,HIGH);
					
					qtyA = 0;
					qtyB = 0;
					qtyC = 0;
			}
			else if ((bot.message[i][5] == "getuptime"))
			{
				unsigned long runtime = millis() / 1000;
				bot.sendMessage(bot.message[i][4], "Uptime: " + String(runtime) + "sec.", "");
			}
			else 
			{
				bot.sendMessage(bot.message[i][4], "Command unknown.", "");
			}
		}
		// If any other person sends something to your bot, let them know they are not allowed to do so!!
		else 
		{
			String senderID = bot.message[i][4];
			bot.sendMessage(bot.message[i][4], "Access denied. Your ID: " + senderID, "");
			//
		}
	}
	bot.message[0][0] = "";
	delay(500);
}
