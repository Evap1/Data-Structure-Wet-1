
#include "Movie.h"





//________________________Constructor_________________________

// Consumer must check if ID already exists

/// @brief 
/// @param id Consumer must check if ID already exists
/// @param genreType movie's Genre
/// @param viewsCount movies views till now
/// @param isVip is the movie for vip's only?
Movie::Movie(int id, Genre genreType, int viewsCount, bool isVip) : 
    movieId(id), genre(genreType), vipOnly(isVip)
{
    views = viewsCount;
    rate = 0;
    ratersCont = 0;
}



 // _______________________________________________Functions______________________________________________

/// @brief add views to the movie's views count
/// @param count assume that count is positive
StatusType Movie::add_views(int count)
{
    if (count >= 0)
    {
        views += count;
        return StatusType::SUCCESS;
    }
    return StatusType::INVALID_INPUT;
}


/// @brief adds one more user's rating to the movie's rating calculation
/// @param rate assume that in range 1-100
StatusType Movie::add_rating(int rated)
{
    if ((rated >= 0) && (rated <= 100))
    {
        rate = (ratersCont * rate + rated) / (ratersCont + 1);
        ratersCont++;
        return StatusType::SUCCESS;
    }
    return StatusType::INVALID_INPUT;
}

/// @brief prints Movie's properties
void Movie::printMovie() const
{
    std::cout << "Movie Id: " << movieId << "\n Genre: " << (int)genre << "\n VipOnly: " << vipOnly 
        << "\n Rate" << rate << "\n Views: " << views << std::endl;
}




// _________________________________________________GETTERS______________________________________________________________

int Movie::getMovieId() const{
    return movieId;
}

Genre Movie::getGenre() const{
    return genre;
}

bool Movie::isVipOnly() const{
    return vipOnly;
}

int Movie::getViews() const{
    return views;
}

double Movie::getRate() const{
    return rate;
}





// ___________________________________________Operator Overloading__________________________________________


/// @brief Operator > for movies
/// @param movie1 
/// @param movie2 
/// @return 
bool operator>(const Movie &movie1, const Movie &movie2)
{
    // if we are in a search of a movie by ID with a temp movie
    if(movie1.getGenre() == Genre::NONE || movie2.getGenre() == Genre::NONE)
        return (movie1.getMovieId() > movie2.getMovieId());

    // regular movie equivalent
    if(movie1.getRate() > movie2.getRate())
        return true;
    else if (movie1.getRate() == movie2.getRate())
    {
        if (movie1.getViews() > movie2.getViews())
            return true;
        else if (movie1.getViews() == movie2.getViews())
        {
            if (movie1.getMovieId() > movie2.getMovieId())
                return true;
        }
    }

    return false;
}

bool operator<(const Movie& movie1, const Movie& movie2)
{
    return (movie2 > movie1);
}



 /// @brief check if the movies are equals by their Id
 /// @param movie1
 /// @param movie2
 /// @return
 bool operator==(const Movie &movie1, const Movie &movie2)
 {
     if(!(movie1>movie2 || movie1<movie2))
         return true;
     return false;
 }


