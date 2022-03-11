package main

import (
	"fmt"
	"net"
	"time"
)

func main() {
	// 仅使用 ipv4
	for i := 0; i < 10; i++ {
		go func() {
			_, err := net.Dial("tcp4", "localhost:22")
			if err != nil {
				fmt.Println(err)
			}
			time.Sleep(time.Minute * 10)
		}()
	}
	time.Sleep(time.Minute * 10)
}
