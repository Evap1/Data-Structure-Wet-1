
#ifndef WET1_MOVIE_H
#define WET1_MOVIE_H

#include "wet1util.h"
#include<iostream>

class IdSearch;



class Movie{

    private:
    
    int movieId;
    Genre genre;
    bool vipOnly;
    int views;
    double rate;
    int ratersCont;

public:

    // friend class IdSearch;

    Movie(int id, Genre genreType, int viewsCount, bool isVip);
    Movie(const Movie& movie) = delete;
    ~Movie() =  default;

    int getMovieId() const;
    Genre getGenre() const;
    bool isVipOnly() const;
    int getViews() const;
    double getRate() const;

    void add_views(int count);
    void add_rating(int rate);

    void printMovie() const;


};



// Equalitu
enum struct Equality {
    LESS           = 0,
    EQUAL            = 1,
    GREATER           = 2,
};

class IdSearch{

    
    public:
    IdSearch() = default;
    bool operator()(const Movie& movie1, const Movie& movie2, Equality kind) const
    {
        if(kind == Equality::LESS)
            return movie1.getMovieId() < movie2.getMovieId();   

        if(kind == Equality::EQUAL)
            return movie1.getMovieId() == movie2.getMovieId();

        if(kind == Equality::GREATER)
            return movie1.getMovieId() > movie2.getMovieId();
    }
};





#endif //WET1_MOVIE_H
