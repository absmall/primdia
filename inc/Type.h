#ifndef __TYPE_H__
#define __TYPE_H__

#include <config/common.h>
#include <map>

class Type
{
public:
	Type(const std::string &s);

	virtual ~Type()=0;

	static Type *getType(const std::string &s);

	static void deinitialize(void);

	virtual Value *parse(const std::string &s) const =0;

	/**
	 * Register a binding to receive input from
	 * a view. This may require user interaction
	 * that would block the UI, so it executes
	 * asynchronously.
	 * @param v The view to be used for display
	 *          purposes if user input is required
	 * @param b The binding for which to set the
	 *          value
	 */
	virtual void input(View *v, Binding *b) const=0;

	std::string name;
private:
	static void registerType(const std::string &name, Type *description);
};

#endif /* __TYPE_H__ */
