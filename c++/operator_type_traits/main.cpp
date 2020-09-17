
struct A{
	A& operator+=(const A& a){return *this;};
};
struct B{
	B& operator+=(const B& b) = delete;
	B operator+(const B& b){
		B ret;
		// some stuff
		return ret;
	}
};

int main(){
	return 0;
}
