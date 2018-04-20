package main

// sync is needed for groups and mutexes
// we add it only if concurrent or critical 
// keywords where in code
import "sync"   
// fmt is for print support
// we add it if any print was present in code
import "fmt"

// For functions we make a function creator first
func __CONSTRUCT__func_exmpl() func() {
	// First is function prelude
	// Then we return the function
	return func() {
		// print defaults to format
		fmt.Printf("Just an example")
	}
}
//Then we make a function under proper name
var func_exmpl = __CONSTRUCT__func_exmpl()

func ___CONSTRUCT__test() func(*int64) {
	// for functions with critical keyword
	// prelude creates one mutex for each critical block
	// for function to lock on in critical sections
	m1 := &sync.Mutex{}
	// As we pass everything by ref
	// We need to remember about it in Go
	return func(a *int64) {
		// lock/Unlock make critical scope
		m1.Lock()
		a = a + 1
		m1.Unlock()
	} 
}
var test = ___CONSTRUCT__test()

// We need to track the returns 
// If there is one it needs to be
// added to the function definition 
func __CONSTRUCT__return_in_critical() func(*int64) int64 {
	m1 := &sync.Mutex{}
	return func(a *int64) int64 {
		m1.Lock()
		a = a + 1 
		// here we cought return return a - 1
		m1.Unlock()
		// we put it after unlock
		return a - 1
	}
}
var return_in_critical = __CONSTRUCT__return_in_critical()

func __CONSTRUCT__multiple_critical(a) func(*int64) {
	// One mutex for each of critical blocks
	m1 := &sync.Mutex{}
	m2 := &sync.Mutex{}
	return func(a *int64) {
		// A first section
		fmt.Printf("Starting...")
		m1.Lock()
		a = a + 1
		m1.Unlock()
		fmt.Printf("Added...")
		// And a second one
		m2.Lock()
		a = a - 1
		m2.Unlock()
	}
}
var multiple_critical = __CONSTRUCT__multiple_critical()


// main is special, we parse it to normal golang main
func main() {
	// Function call
	func_exmpl()
	// Only type we use is int64
	var a int64 = 1
	// For concurrent blocks we make a new scope
	{
		// with wait group at the beggining
		var wg sync.WaitGroup
		// For each expression in concurrent block
		// we create a new gorutine
		// and add 1 to a wait group
		wg.Add(1) 
		go func() {
			// we need to say the wait group that we are done
			defer wg.Done()
			// remember to pass a reference not a copy
			test(&a)
		}()
		wg.Add(1)
		go func() {
			defer wg.Done()
			test(&a)
		}()
		wg.Add(1)
		go func() {
			defer wg.Done()
			test(&a)
		}()
		// as concurrent is sequential 
		// we want to wait for it's end
		// wich ends when all gorutines ends
		// we use groups to know that 
		wg.Wait()
	}
	fmt.Printf("Expected 4 got %v", a)
}
