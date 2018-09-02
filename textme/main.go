package main

import (
	"github.com/tarm/serial"
	"log"
	"github.com/julienschmidt/httprouter"
	"net/http"
	"io/ioutil"
	"net/url"
)

type AppConfig struct {
	Port *serial.Port
}

var GlobalConfig AppConfig = AppConfig{}

func ReceivedSMS(w http.ResponseWriter, r *http.Request, ps httprouter.Params) {
	smsForm, _ := ioutil.ReadAll(r.Body)
	smsValues, err := url.ParseQuery(string(smsForm))

	if err == nil {
		message := smsValues.Get("Body") + "\r\n"
		GlobalConfig.Port.Write([]byte(message))
		log.Println("Received: " + message)
	} else {
		log.Println(err.Error())
	}
}

func main() {
	c := &serial.Config{
		Name: "COM3",
		Baud: 9600,
	}

	port, err := serial.OpenPort(c)

	if err == nil {
		GlobalConfig.Port = port
	} else {
		log.Println(err.Error())
	}

	r := httprouter.New()
	r.POST("/twilio/inbound", ReceivedSMS);

	log.Fatal(http.ListenAndServe(":3000", r))
}