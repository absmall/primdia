#ifndef __DOCUMENT_H__
#define __DOCUMENT_H__

#include <config/common.h>

/**
 * The document contains all data from one file to be displayed in one or more
 * views
 */

#include <set>
#include <list>

#include <NodeSet.h>

class Document : public NodeSet
{
public:
	/**
	 * The different kinds of updates that may be performed.
	 * They are used to describe the message that may be passed.
	 */
	enum UpdateCommands
	{
		/** An attribute was added to some binding in the document */
		AddBinding = 0,
		/** A new interface was instantiated in the document */
		AddNode,
		/** An interface was deleted from the document */
		RemoveNode,
		/** An attribute was removed some binding in the document */
		RemoveAttribute,
		/** A value was set for some binding in the document */
		SetValue,
		/** A binding had its value removed */
		UnsetValue,
		/** Sentinel value. There may be more messages that
		 * are UI specific
		 */
		MAX
	};

	/**
	 * Default constructor.
	 */
	Document();

	/**
	 * Default destructor.
	 */
	virtual ~Document();

	/**
	 * Register a view that displays this document. Whenever a change occurs in
	 * the document, the change will be propagated to all views
	 * @param v The view to register with this document
	 */
	void registerView(View *v);

	/**
	 * Unregister a view, indicating that the view should no longer be alerted
	 * when changes are made to the document
	 * @param v The view to release
	 */
	void unRegisterView(View *v);

	/**
	 * Retrieve the name of the document
	 * @return String describing the name
	 */
	const std::string &getName();

	/**
	 * Retrieve the list of views watching the document
	 * @return The list of views
	 */
	const std::list<View *> &getViews();

	/**
	 * Retrieve the list of all active documents
	 * @return The list of documents
	 */
	static const std::list<Document *> &getDocuments();

	/**
	 * Add a new aggregate to the document
	 * @param type The tool to instantiate to produce the aggregate
	 * @return A reference to the aggregate
	 */
	Interface *addNode(const Tool *type);

	/**
	 * Retrieve the list of nodes in the document
	 * @return The list of nodes
	 */
	const std::set<Interface *> &getNodes(void) const;

	/**
	 * Remove an instantiation of a tool from the document
	 * @param n The instantiation to remove
	 */
	void unregisterNode(Interface *n);

	/**
	 * Pass a message to the document that a particular type of update was performed
	 * so the views must be notified
	 * @param command What kind of update happened
	 * @param attribute A reference to an affected attribute
	 * @param node A reference to an affected node
	 * @param binding A reference to an affected binding
	 */
	virtual void update(int command, Attribute *attribute, Interface *node, Binding *binding);

	/**
	 * Replace the contents of the document with the contents of a file.
	 * @param name The file to load
	 */
	void load(const std::string &name);

	/**
	 * Save the nodes to a file. Try to
	 */
	void save(void);

	/**
	 * Save the nodes to a file.
	 * @param name The name of the file
	 */
	void saveAs(const std::string &name);

	/**
	 * Check whether the document has been modified since it was last
	 * saved.
	 * @return true if modifications have occured, false otherwise
	 */
	bool isModified(void) const;

private:
	static void updateDocumentList();
	std::string makeTemporaryDocumentName();
	std::list<View *> views;
	bool has_name;
	bool modified;
	std::string name;
	static int newDocuments;
	static std::list<Document *> documentList;
};


#endif /* __DOCUMENT_H__ */
