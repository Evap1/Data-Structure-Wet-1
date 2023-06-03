//
// Created by Eva Poluliakhov on 24/05/2023.
//

#ifndef WET1_GROUP_H
#define WET1_GROUP_H
#include "wet1util.h"
#include "User.h"
#include "AVL.h"
//TODO: make sure < > = operator for  user* !!!!

class Group{
private:

    int groupId;
    bool isVip;
    int numOfVIP;
    bool isTemp;
    TreeNode<User*>* members;
    int sumViewsAsGroup [(int)Genre::NONE + 1] = {0};
    int numOfMoviesWatched [(int)Genre::NONE + 1] = {0};

public:
    explicit Group(int id);
    Group(int id, bool Temp);
    Group(const Group& group);
    ~Group();

    Group& operator=(Group &other);

    StatusType add_user(User *member, UserPtrCompare ptrCompare);
    int get_member_count();
    int get_id() const;
    int get_views_per_genre(Genre type) const;
    int get_movies_as_group(Genre type) const;
    TreeNode<User*> *get_members() {return members; };
    bool is_vip();
    void empty_group_aux(  Node<User*>* v);
    StatusType free_members();
    void set_views_per_movie(Genre type);
    void set_views_per_movie_user_watch(Genre type);
    void set_views(Genre genre, int amount);
    void remove_vip();
    void set_members(TreeNode<User*>* ptr);
    void get_members_out_of_group(Node<User*>* currentRoot);


};

// ___________________________________________Operator Overloading__________________________________________

bool operator>(const Group &group1, const Group &group2);
bool operator<(const Group& group1, const Group& group2);
bool operator==(const Group& group1, const Group& group2);

#endif //WET1_GROUP_H
