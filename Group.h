//
// Created by Eva Poluliakhov on 24/05/2023.
//

#ifndef WET1_GROUP_H
#define WET1_GROUP_H
#include "wet1util.h"
#include "User.h"
#include "AVL.h"
class Group{
private:
    int groupId;
    bool isVip;
    int viewsAsGroup[(int)Genre::NONE] = {0};
    TreeNode<User> members;

public:
    explicit Group(int id);
    void add_user(const User& member);
    int get_member_count();
    int get_id() const;
    void set_views_per_movie(Genre type);
};

bool operator>(const Group &group1, const Group &group2);
bool operator<(const Group& group1, const Group& group2);
bool operator==(const Group& group1, const Group& group2);

#endif //WET1_GROUP_H
