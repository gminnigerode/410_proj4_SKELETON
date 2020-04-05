#include <mutex>
#include "../includes/baker.h"
#include "../includes/externs.h"
using namespace std;

mutex mtx;

Baker::Baker(int id):id(id)
{
}

Baker::~Baker()
{
}

void Baker::bake_and_box(ORDER &anOrder) {
}

void Baker::beBaker() {
}
