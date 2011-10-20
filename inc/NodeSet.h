#ifndef __NODESET_H__
#define __NODESET_H__

#include <config/common.h>

/**
 * Contains a set of nodes. This could represent an entire document, or some
 * subset of one.
 */

#include <set>

class NodeSet
{
public:
	/**
	 * Load the nodes from a file.
	 * @param name The name of the file
	 */
	void load(const std::string &name);

	/**
	 * Save the nodes to a file.
	 * @param name The name of the file
	 */
	void saveAs(const std::string &name);

	/**
	 * Save the node in the format of a tool to a file.
	 * @param name The name of the file
	 */
	void saveTool(const std::string &name);

protected:
	/**
	 * This stores all the interfaces instantiated into the document.
	 */
	std::set<Interface *> nodes;
};

#endif /* __NODESET_H__ */
