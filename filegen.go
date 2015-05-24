package main

import (
	"math/rand"
	"bytes"
	"fmt"
	"os"
	"io"
)

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func writetofile(f *os.File, word string) {
	_, err := f.WriteString(word)
	check(err)
}

func test(str []string, prob int) (bool, string) {
	P := rand.Intn(100)
	if prob > P {
		s:= rand.Intn(len(str)- 1)
		return true, str[s]
	}
	return false, ""
}

func copyfile(r *os.File, times int) {
	w, err := os.Create("output.txt")
	if err != nil {
		panic(err)
	}
	defer w.Close()

	var writen int64
	for i := 0; i < times; i++ {
		// copy 
		r.Seek(0,0)
		n, err := io.Copy(w, r)
		if err != nil {
			panic(err)
		}
		writen = writen + n
	}

	fmt.Printf("Copied %v bytes\n",  writen)
}

var letters = []string{"q","w","e","r","t","y","u","i","o","p","a","s","d","f","g","h","j","k","l","z","x","c","v","b","n","m"}
var nums = []string{"1","2","3","4","5","6","7","8","9","0"}
var puncts = []string{"`","-","=","~","!",",",".","?",":",";"}
var russ = []string{"й","ц","у","к","е","н","г","ш","щ","з","х","ъ","ф"}
var spaces = []byte{0x20, 0x9, 0xa, /*0xb, 0xc,*/ 0xd} // ' ' \t \n \v \f \r

func main() {
	rand.Seed(42)

	f, err := os.Create("input.txt")
	check(err)
	defer f.Close()

	letterslen := len(letters)
	spaceslen := len(spaces)

	// set probability
	punctProb := 20
	numProb := 2
	rusProb := 3

	wordsNum := 1000000

	var buf bytes.Buffer

	for i:=0; i < wordsNum; i++{
		len := rand.Intn(9) + 1
		spacelen := rand.Intn(2) + 1
		// word
		for j:=0; j < len; j++{
			charN:= rand.Intn(letterslen-1)
			buf.WriteString(letters[charN])
			if b,s := test(nums, numProb); b{
				buf.WriteString(s)
			}
			if b,s := test(russ, rusProb); b{
				buf.WriteString(s)
			}
		}
		if b,s := test(puncts, punctProb); b{
			buf.WriteString(s)
		}
		for j:=0; j < spacelen; j++{
			spaceN:= rand.Intn(spaceslen-1)
			buf.WriteString(string(spaces[spaceN]))
		}
		writetofile(f,buf.String())
		buf.Reset();
	}

	copyfile(f, 2)
}
