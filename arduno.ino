#include <Arduino.h>
#include <WiFiNINA.h>
#include <Firebase_Arduino_WiFiNINA.h>

// Firebase configuration details
#define FIREBASE_HOST "manav-8eb13-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "VBlLyj11FrImmpxSX4R4qwePtqdERXABAHbPuN7W"

// WiFi credentials
#define WIFI_SSID "manav"
#define WIFI_PASSWORD "manav123"

// Firebase data path to store the LED status
const String firebasePath = "/light/led";

// Define LED pins
const int RED_LED_PIN = 5;
const int YELLOW_LED_PIN = 6;
const int GREEN_LED_PIN = 7;

// Firebase data object
FirebaseData firebaseData;

void setup()
{
  Serial.begin(9600); // Start serial communication
  pinMode(RED_LED_PIN, OUTPUT);   // Set RED LED pin as output
  pinMode(YELLOW_LED_PIN, OUTPUT); // Set YELLOW LED pin as output
  pinMode(GREEN_LED_PIN, OUTPUT);  // Set GREEN LED pin as output

  // Connect to Wi-Fi network
  connectToWiFi();

  // Initialize Firebase connection
  initializeFirebase();

  // Set initial Firebase status to "OFF"
  setFirebaseStatus("OFF");
}

void loop()
{
  // Retrieve the LED status from Firebase
  String status = getFirebaseStatus();

  // Control LEDs based on the Firebase status
  if (status == "RED")
  {
    controlLEDs(HIGH, LOW, LOW); // Turn on RED LED
    Serial.println("Turning the RED LED ON");
  }
  else if (status == "YELLOW")
  {
    controlLEDs(LOW, HIGH, LOW); // Turn on YELLOW LED
    Serial.println("Turning the YELLOW LED ON");
  }
  else if (status == "GREEN")
  {
    controlLEDs(LOW, LOW, HIGH); // Turn on GREEN LED
    Serial.println("Turning the GREEN LED ON");
  }
  else if (status == "OFF")
  {
    controlLEDs(LOW, LOW, LOW); // Turn off all LEDs
    Serial.println("Turning all LEDs OFF");
  }
  else
  {
    Serial.println("Unknown status received from Firebase: " + status); // Handle unknown status
  }

  delay(1000); // Wait for 1 second before next iteration
}

// Connect to Wi-Fi
void connectToWiFi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("."); // Print dots while connecting
    delay(1000);
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("Local IP Address: ");
  Serial.println(WiFi.localIP()); // Display IP address once connected
  Serial.println();
}

// Initialize Firebase connection
void initializeFirebase()
{
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASSWORD); // Initialize Firebase with credentials
  Firebase.reconnectWiFi(true); // Automatically reconnect Wi-Fi if needed
}

// Set the LED status in Firebase
void setFirebaseStatus(const String &status)
{
  if (Firebase.setString(firebaseData, firebasePath, status))
  {
    Serial.println("Firebase status updated to: " + status); // Successfully updated Firebase status
  }
  else
  {
    Serial.println("Failed to update Firebase status. Error: " + firebaseData.errorReason()); // Error handling
  }
}

// Retrieve the LED status from Firebase
String getFirebaseStatus()
{
  if (Firebase.getString(firebaseData, firebasePath))
  {
    return firebaseData.stringData(); // Return the status if successfully retrieved
  }
  else
  {
    Serial.println("Failed to retrieve Firebase status. Error: " + firebaseData.errorReason()); // Error handling
    return "ERROR"; // Return "ERROR" if retrieval fails
  }
}

// Control LEDs based on received status
void controlLEDs(int red, int yellow, int green)
{
  digitalWrite(RED_LED_PIN, red);   // Control RED LED
  digitalWrite(YELLOW_LED_PIN, yellow); // Control YELLOW LED
  digitalWrite(GREEN_LED_PIN, green);   // Control GREEN LED
}
