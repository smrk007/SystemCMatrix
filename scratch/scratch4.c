#define TEST_A(arg,...) a = arg; TEST_B(__VA_ARGS__);
#define TEST_B(arg,...) a = arg; TEST_A(__VA_ARGS__);

int main() {
	int a;
	TEST_A(1,2,3,4);
	return 0;
}
