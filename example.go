package main

// sync is needed for groups and mutexes
// we add it only if concurrent or critical 
// keywords where in code
import "sync"   
// fmt is for print support
// we add it if any print was present in code
import "fmt"

// Simple functions are parsed to golang functions
func func_exmpl() {
	// But we don't define them
	// We return them !
	return func() {
		fmt.Printf("Just an example")
	}
}


func test() {
	//creates mutex for functions to
	//lock on in critical sections
	m := &sync.Mutex{}
	//As we pass everything by ref
	//We need to remember about it in Go
	return func(a *int64) {
		// lock/Unlock make critical scope
		m.Lock()

		a = a + 1

		m.Unlock()
	} 
}

// We need to track the returns 
// If there is one it needs to be
// added to the function definition 
func return_in_critical() int64 {
	m := &sync.Mutex{}
	return func(a *int64) {
		m.Lock()
		a = a + 1 
		// here we cought return return a - 1
		m.Unlock()
		// we put it after unlock
		return a - 1
	}
}

func main() {
	//Only type we use is int64
	int64 a = 1
	//For concurrent blocks we make a new scope
	{

	}
	//as concurrent is sequential 
	// we want to wait for it's end
	// wich ends when all gorutines ends
	// we use groups to know that 
}
