package main

import (
	"fmt"
	"net"
	"time"
)

func main() {
	for i := 0; i < 2000; i++ {
		go connect()
	}
	time.Sleep(time.Minute * 10)
}
func connect() {
	_, err := net.Dial("tcp4", "10.211.55.15:9090")
	if err != nil {
		fmt.Println(err)
	}
}
