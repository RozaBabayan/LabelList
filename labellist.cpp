#include "labellist.h"

#include <cassert>
#include <iostream>

// Complete - Do not alter
LabelList::MsgNode::MsgNode(const MsgType& msg) : msg_(msg) {
    // Every message is part of the "all" (index 0) label
    labelled_.push_back(true);
    next_.push_back(nullptr);
    prev_.push_back(nullptr);
}

// Complete - Do not alter
LabelList::MsgToken::MsgToken() : node_(nullptr), list_(nullptr) {}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgToken::MsgToken(MsgNode* node, LabelList* list)
// : node_(node),
//   list_(list)

// Add any initializers here
{
    node_ = node;
    list_ = list;
}

// To be completed - must adhere to any requirements in the .h file
bool LabelList::MsgToken::operator==(const MsgToken& rhs) const { return (this->node_ == rhs.node_); }

// Complete - Do not alter
bool LabelList::MsgToken::operator!=(const MsgToken& rhs) const { return !operator==(rhs); }

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgToken::operator bool() const { return (this->isValid()); }

// Complete - Do not alter
bool LabelList::MsgToken::isValid() const { return this->node_ != nullptr; }

// Complete - Do not alter
LabelList::MsgToken& LabelList::MsgToken::next(const LabelType& label) {
    size_t index = this->list_->findLabelIndex(label);
    if (false == this->isLabelMember(index)) {
        throw std::invalid_argument("Node is not part of the given label");
    }
    this->node_ = this->node_->next_[index];

    // Should return reference to self
    return *this;
}

// Complete - Do not alter
LabelList::MsgToken& LabelList::MsgToken::prev(const LabelType& label) {
    size_t index = this->list_->findLabelIndex(label);
    if (false == this->isLabelMember(index)) {
        throw std::invalid_argument("Node is not part of the given label");
    }
    this->node_ = this->node_->prev_[index];

    // Should return reference to self
    return *this;
}

// Complete - Do not alter
bool LabelList::MsgToken::isLabelMember(const LabelType& label) const {
    size_t index = this->list_->findLabelIndex(label);
    return isLabelMember(index);
}

// Complete - Do not alter
bool LabelList::MsgToken::isLabelMember(size_t index) const {
    return this->isValid() && index != INVALID_LABEL && index < this->node_->labelled_.size()
           && this->node_->labelled_[index];
}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgType const& LabelList::MsgToken::msg() const {

    if (!this->isValid()) {
        throw std::logic_error("Invalid message!");
    }
    return (this->node_->msg_);
}
// To be completed - must adhere to any requirements in the .h file
LabelList::MsgType& LabelList::MsgToken::msg() {
    if (!this->isValid()) {
        throw std::logic_error("Invalid message!");
    }
    return (this->node_->msg_);
}

// To be completed - must adhere to any requirements in the .h file
std::ostream& operator<<(std::ostream& ostr, const LabelList::MsgToken& token) {
    if (token.isValid()) {
        ostr << token.node_->msg_;
    }
    return ostr;
}

// Static constant
const LabelList::MsgToken LabelList::end_(nullptr, nullptr);

// To be completed - must adhere to any requirements in the .h file
LabelList::LabelList()
// Add any initializers here
{
    // Construct a new Label List object with an "all" label
    labels_.push_back("all");
    heads_.push_back(nullptr);
    tails = nullptr;
}

// To be completed - must adhere to any requirements in the .h file
LabelList::~LabelList() {
    // Destroy the Label List object
    clear();
}

// To be completed - must adhere to any requirements in the .h file+
void LabelList::clear() {
    // delete all the nodes
    MsgNode* temp;
    while (heads_[0] != nullptr && tails != nullptr) {
        temp = this->tails;
        this->tails = tails->prev_[0];
        delete temp;
    }
    for (long unsigned int i = 0; i < labels_.size(); i++) {
        this->heads_[i] = nullptr;
    }
}

// To be completed - must adhere to any requirements in the .h file+
bool LabelList::empty() const { return ((this->tails == nullptr) && (this->heads_[0] == nullptr)); }

// Complete
size_t LabelList::size() const {
    MsgNode* n = this->heads_[0];
    size_t cnt = 0;
    while (n != nullptr) {
        cnt++;
        n = n->next_[0];
    }
    return cnt;
}

// To be completed - must adhere to any requirements in the .h file+
LabelList::MsgToken LabelList::add(const MsgType& msg) {

    MsgNode* nodePtr = new MsgNode(msg);  // allocate new memory
    if (!empty()) {
        // the list is not empty
        this->tails->next_[0] = nodePtr;
        nodePtr->prev_[0] = tails;
        nodePtr->next_[0] = nullptr;
        this->tails = nodePtr;

    } else {
        // the list is empty
        nodePtr->next_[0] = nullptr;
        this->tails = nodePtr;
        this->heads_[0] = this->tails;
    }

    MsgToken newMsgToken(nodePtr, this);
    return (newMsgToken);  // return MsgToken to the newly added message
}

// To be completed - must adhere to any requirements in the .h file
void LabelList::remove(const MsgToken& token) {
    /*Removes and deletes the message referenced by the given token
     updating all lists that this message was a member of.
    If an invalid token is passed, do NOTHING and simply return.*/
    MsgNode* remove_node = token.node_;
    if (!token.isValid()) {
        return;
    }
    long unsigned int i = 0;
    while (i < labels_.size()) {
        if (token.isLabelMember(i)) {
            if (token.node_->prev_[i] == nullptr && token.node_->next_[i] == nullptr) {
                heads_[i] = nullptr;
                if (i == 0) {
                    tails = nullptr;
                }

            } else if (token.node_->next_[i] == nullptr) {
                token.node_->prev_[i]->next_[i] = nullptr;
                token.node_->prev_[i] = nullptr;
            } else if (token.node_->prev_[i] == nullptr) {
                heads_[i] = token.node_->next_[i];
                token.node_->next_[i]->prev_[i] = nullptr;
                token.node_->next_[i] = nullptr;

            } else {
                token.node_->prev_[i]->next_[i] = token.node_->next_[i];
                token.node_->next_[i]->prev_[i] = token.node_->prev_[i];
                token.node_->next_[i] = nullptr;
                token.node_->prev_[i] = nullptr;
            }
            token.node_->labelled_[i] = false;
        }
        i++;
    }

    delete remove_node;
}

// To be completed - must adhere to any requirements in the .h file
void LabelList::label(const MsgToken& token, const LabelType& label) {
    if (!token.isValid()) {
        throw std::invalid_argument("Bad token in label()");
    }
    // find the index
    size_t index = this->findLabelIndex(label);

    if (index == INVALID_LABEL) {
        // create a new label
        labels_.push_back(label);
        heads_.push_back(token.node_);
        index = labels_.size() - 1;
        token.node_->next_.resize(heads_.size());
        token.node_->labelled_.resize(heads_.size(), false);
        token.node_->prev_.resize(heads_.size());

        heads_.back()->labelled_[index] = true;
        heads_.back()->next_[index] = nullptr;
        heads_.back()->prev_[index] = nullptr;
    }
    // after this runs, we are guarantee that index is an index of a valid label

    else if (token.isLabelMember(index)) {
        return;
    }  // if message already labeled index

    // otherwise insert (msg, index);
    // Here is my logic: insert message into label index
    // treat the label as a linked list (insert that msg into the linked list)
    // make sure the order where we insert the message matters
    // example if msg comes before A msg in its category, it should comes before that msg all category

    else {

        // resize  next,prev, labelled
        token.node_->next_.resize(index + 1);
        token.node_->labelled_.resize(index + 1, false);
        token.node_->prev_.resize(index + 1);

        token.node_->labelled_[index] = true;  // true membership flag

        MsgNode* tempright = tails;     // to find  after tokennode
        MsgNode* templeft = heads_[0];  // to find before tokennode
        MsgNode* after = nullptr;
        MsgNode* before = nullptr;

        // get the left message
        while ((templeft != token.node_)) {
            if ((templeft->labelled_.size() >= index + 1)) {
                if ((templeft->labelled_[index] == true)) {
                    before = templeft;
                }
            }
            templeft = templeft->next_[0];
            if (templeft == nullptr) {
                before = nullptr;
                break;
            }
        }

        // get the right message
        while (tempright != token.node_) {
            if ((tempright->labelled_.size() >= index + 1)) {
                if ((tempright->labelled_[index] == true)) {
                    after = tempright;
                }
            }
            tempright = tempright->prev_[0];
            if (tempright == nullptr) {
                after = nullptr;
                break;
            }
        }

        // case 1 when  after and before are null
        if (after == nullptr && before == nullptr) {

            token.node_->next_[index] = after;
            token.node_->prev_[index] = before;
            heads_[index] = token.node_;

        } else if (after == nullptr && before != nullptr) {

            token.node_->next_[index] = after;
            token.node_->prev_[index] = before;
            before->next_[index] = token.node_;

        } else if (before == nullptr && after != nullptr) {

            token.node_->next_[index] = after;
            token.node_->prev_[index] = before;
            after->prev_[index] = token.node_;
            heads_[index] = token.node_;

        } else  // if after and before is not null
        {

            after->prev_[index] = token.node_;
            token.node_->next_[index] = after;  // after is=token.next
            token.node_->prev_[index] = before;
            before->next_[index] = token.node_;
        }
    }
}

// To be completed - must adhere to any requirements in the .h file
void LabelList::unlabel(const MsgToken& token, const LabelType& label) {
    if (!token.isValid()) {
        throw std::invalid_argument("Bad token in label()");
    }
    size_t i = getLabelIndex(label);
    if (token.isLabelMember(i) && i != 0) {
        token.node_->labelled_[i] = false;
        if (token.node_->prev_[i] == nullptr && token.node_->next_[i] == nullptr) {
            heads_[i] = nullptr;
            if (i == 0) {
                tails = nullptr;
            }

        } else if (token.node_->next_[i] == nullptr) {
            token.node_->prev_[i]->next_[i] = nullptr;
            token.node_->prev_[i] = nullptr;
        } else if (token.node_->prev_[i] == nullptr) {
            heads_[i] = token.node_->next_[i];
            token.node_->next_[i]->prev_[i] = nullptr;
            token.node_->next_[i] = nullptr;

        } else {
            token.node_->prev_[i]->next_[i] = token.node_->next_[i];
            token.node_->next_[i]->prev_[i] = token.node_->prev_[i];
            token.node_->next_[i] = nullptr;
            token.node_->prev_[i] = nullptr;
        }
    }
}

// Complete - Do not alter
LabelList::MsgToken LabelList::find(const MsgType& msg, const LabelType& label) {
    // See if the label exists, will throw if not
    size_t level = this->getLabelIndex(label);
    MsgNode* n = this->heads_[level];
    while (n != nullptr) {
        if (n->msg_ == msg) {
            return MsgToken(n, this);
        }
        n = n->next_[level];
    }
    return end();
}

// To be completed - must adhere to any requirements in the .h file
LabelList::MsgToken LabelList::find(size_t index, const LabelType& label) {

    size_t search_index = this->findLabelIndex(label);
    // See if the label exists, will throw if not
    if (INVALID_LABEL == search_index) {
        throw std::out_of_range("None existenet label is provided!");
    }
    size_t newIndex = 0;
    MsgNode* l = this->heads_[search_index];
    while (l != nullptr) {

        if (newIndex == index) {
            return MsgToken(l, this);
        }
        newIndex++;
        l = l->next_[search_index];
        if (l == nullptr) {
            return end();
        }
    }
    return end();
}

// Complete - Do not alter
LabelList::MsgToken const& LabelList::end() const { return end_; }

// Complete - Do not alter
void LabelList::print(std::ostream& ostr, const LabelType& label, char separator) const {
    size_t level = this->getLabelIndex(label);
    MsgNode* n = this->heads_[level];
    while (nullptr != n) {
        ostr << n->msg_ << separator;
        n = n->next_[level];
    }
    ostr << std::endl;
}

// To be completed - must adhere to any requirements in the .h file
size_t LabelList::findLabelIndex(const LabelType& label) const {
    long unsigned int i = 0;
    while (i < labels_.size()) {
        while (label == labels_[i]) {
            return i;
        }
        i++;
    }

    return INVALID_LABEL;
}

// Complete - Do not alter
size_t LabelList::getLabelIndex(const LabelType& label) const {
    size_t retval = this->findLabelIndex(label);
    if (INVALID_LABEL == retval) {
        throw std::out_of_range("Label doesn't exist");
    }
    return retval;
}
