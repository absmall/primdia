#include <Binding.h>
#include <Point2D.h>
#include <DistanceType.h>

using namespace std;

class ConsoleDistanceType : public DistanceType
{
public:
	/**
	 * Get a value from the user
	 * @param v A view to be used in some
	 *          way to retrieve a value.
	 * @param b A binding to initialize with the value.
	 */
	void input(View *v, Binding *b) const;
};

static ConsoleDistanceType instance;

void ConsoleDistanceType::input(View *v, Binding *b) const
{
}
