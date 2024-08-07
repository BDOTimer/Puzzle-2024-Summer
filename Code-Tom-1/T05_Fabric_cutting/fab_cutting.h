struct Fabric_cutting
{
    ///---------------------------|
    /// Базовая метрика.          |
    ///---------------------------:
    float SZCELL = 30;
    
    int foot_min =  2;
    int foot_max = 10;
    int foot_now =  6;
    int foot_act =  4;
    
    float dim    = 1.f; /// Расстояние между треками.
    
    float r_now_intro  = 1.0f;
    float r_now_ext;
    
    sf::Vector2f emptiness; /// Прямоугольная пустотелость внутри доски.
    sf::Vector2f shovel   ; /// Прямоугольный движетель-лопата.
    
    void calc()
    {
        r_now_ext = r_now_intro + SZCELL;
        
        emptiness.x = SZCELL * 4 + dim * 2;
        emptiness.y = SZCELL * foot_act;
        
        shovel.x = SZCELL * 2 + dim * 2;
        shovel.y = emptiness.y;
        
    }
};