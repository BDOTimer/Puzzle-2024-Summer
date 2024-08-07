struct Fabric_cutting
{
    ///---------------------------|
    /// Базовая метрика.          |
    ///---------------------------:
    float SZCELL = 30;  /// Диаметр шаров.
    float dim    = 1.f; /// Расстояние между треками.
    
    ///--------------:
    int foot_now =  6;
    int foot_act =  4;
    
    int foot_min =  2;
    int foot_max = 10;
    ///--------------.
    
    float foot_y;
    
    float r_now_intro;
    float r_now_ext;
    
    sf::Vector2f emptiness; /// Прямоугольная пустотелость внутри доски.
    sf::Vector2f shovel   ; /// Прямоугольный движетель-лопата.
    
    float        s_mover  ; /// Расстояние хода лопаты.
    
    void calc()
    {
        foot_y      = SZCELL * foot_now;
        
        emptiness.x = SZCELL * 4 + dim * 2;
        emptiness.y = SZCELL * foot_act;
        
        shovel.x    = SZCELL * 2 + dim * 2;
        shovel.y    = emptiness.y;
        
        s_mover     = SZCELL + dim;
        
        r_now_intro = SZCELL + float(int(dim) / SZCELL) / 2;
        r_now_ext   = r_now_intro + SZCELL;
    }
};