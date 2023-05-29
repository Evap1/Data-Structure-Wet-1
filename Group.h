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
    int* sumViewsAsGroup;
    int* numOfMoviesWatched;
    //TODO:
    TreeNode<User*>* members;

public:
    // TODO: destructor
    explicit Group(int id);
    ~Group();
    StatusType add_user(User* member);
    int get_member_count();
    int get_id() const;
    int get_views_per_genre(Genre type) const;
    int get_movies_as_group(Genre type) const;

    bool is_vip();
    void empty_group();
    void empty_group_aux(  Node<User*>* v);
    StatusType free_members();
    void set_views_per_movie(Genre type);
    void set_views_per_movie_user_watch(Genre type);
    void set_views(Genre genre, int amount);

};

// ___________________________________________Operator Overloading__________________________________________

bool operator>(const Group &group1, const Group &group2);
bool operator<(const Group& group1, const Group& group2);
bool operator==(const Group& group1, const Group& group2);

#endif //WET1_GROUP_H
