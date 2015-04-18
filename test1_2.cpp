#include<simplecpp>
#include<fstream>
//structure of point and ball is defined
struct Point
{
    double x,y;
    Point(double x1=0,double y1=0)
    {
        x=x1;
        y=y1;
    }
};
struct Ball
{
    const double radius=13.0;
    Circle c;
    Point velocity;
    Ball(double x1=0, double y1=0,double vx=0,double vy=0): c(x1,y1,radius),velocity(Point(vx,vy))
    {}
    void SetBall(double x1=0, double y1=0)//moves the ball to given coordinates from(0,0)
    {
        c.moveTo(x1,y1);
    }
    void fillcolor(int a,int b,int h)//sets the colour of the ball
    {
        c.setColor(COLOR(a,b,h));
        c.setFill(true);
    }
    double dist_bn_ball(Ball *other)//returns distance between two balls
    {
        double t1=c.getX()-(*other).c.getX();
        double t2=c.getY()-(*other).c.getY();
        return sqrt(pow(t1,2)+pow(t2,2));
    }
    void Collision(Ball *b2)//returns velocities after collision and prevents overlapping
    {
        double theta=atan(((*b2).c.getY()-c.getY())/((*b2).c.getX()-c.getX()));
        double vt1=-1*velocity.x*sin(theta)+velocity.y*cos(theta);
        double vt2=-1*(*b2).velocity.x*sin(theta)+(*b2).velocity.y*cos(theta);
        double vn2=velocity.x*cos(theta)+velocity.y*sin(theta);
        double vn1=(*b2).velocity.x*cos(theta)+(*b2).velocity.y*sin(theta);
        velocity.x=-1*vt1*sin(theta)+vn1*cos(theta);
        velocity.y=vt1*cos(theta)+vn1*sin(theta);
        (*b2).velocity.x=-1*vt2*sin(theta)+vn2*cos(theta);
        (*b2).velocity.y=vt2*cos(theta)+vn2*sin(theta);
        velocity.x+=(13-dist_bn_ball(b2)/2)*cos(theta);
        velocity.y+=(13-dist_bn_ball(b2)/2)*sin(theta);
        (*b2).velocity.x-=(13-dist_bn_ball(b2)/2)*cos(theta);
        (*b2).velocity.y-=(13-dist_bn_ball(b2)/2)*sin(theta);
    }
    void moveBall()//displaces the ball in the direction of velocity
    {
        c.move(velocity.x,velocity.y);
        wait(.002);
    }
};
bool check_static(Ball *balls)//returns true if all the balls have zero velocity
{
    for(int i=0; i<11; i++)
    {
        if(balls[i].velocity.x!=0&&balls[i].velocity.y!=0) return false;
    }
    return true;
}
bool check_end(Ball *balll)//returns true if all the balls except cue ball are pocketed as all the balls are set to y=0 after pocketing
{
    for(int i=0; i<10; i++)
    {
        if(balll[i].c.getY()!=0) return false;
    }
    return true;
}
int main()
{
    int highscore,data;
    double msen=40.0;
    //highscore file to keep the count of number of strokes
    ifstream Input("highscore.txt");
    Input>>data;
    highscore=data;
    Input.close();
    int f=0,ne,count=0;//f is the x-coordinate of the ball after pocketing,ne is the no. of last ball pocketed and count is the number of strokes
    double unitvecx,unitvecy;//both these are unit vectors in direction of resultant velocity
    initCanvas("POOL_GAME",1400,800);
    int mode;
    while(1)
    {
        //construction of main window
        Rectangle r1(700,120,150,40);
        r1.setColor(COLOR(96,36,219));
        Text t1(700,120,"NEW GAME");
        Rectangle s1(700,240,150,40);
        s1.setColor(COLOR(96,36,219));
        Text t5(700,240,"OPTION");
        Rectangle r2(700,360,150,40);
        r2.setColor(COLOR(96,36,219));
        Text t2(700,360,"INSTRUCTION");
        Rectangle r3(700,480,150,40);
        r3.setColor(COLOR(96,36,219));
        Text t3(700,480,"HIGH SCORE");
        Rectangle r4(700,600,150,40);
        r4.setColor(COLOR(96,36,219));
        Text t4(700,600,"EXIT");
        while(1)
        {
            //clicking on the button changes the mode in subsequent part
            int clickPos = getClick();
            int cx = clickPos/65536;
            int cy = clickPos % 65536;
            if(625<=cx && cx<=775 && 100<=cy && cy<=140)
            {
                mode=1;
                break;
            }
            else if(625<=cx && cx<=775 &&340<=cy && cy<=380)
            {
                mode=2;
                break;
            }
            else if(625<=cx && cx<=775 && 440<=cy && cy<=480)
            {
                mode=3;
                break;
            }
            else if(625<=cx && cx<=775 &&580<=cy && cy<=620)
            {
                mode=4;
                break;
            }
            else if(625<=cx && cx<=775 && 220<=cy && cy<=260)
            {
                mode=5;
                break;
            }
        }
        Rectangle clear(700,400,1400,800);
        clear.setFill(1);
        clear.setColor(COLOR(255,255,255));
        clear.imprint();
        //instructions--the part to display after you click a particular button
        if(mode==2)
        {
            Text x1(700,320,"Straight Pool:");

            Text t2(700,350," The object of this game is to pocket every ball on the table,");
            Text x3(700,380," with the exception of the cue ball,using as few strokes as possible.");
            Text x4(700,410," The order of pocketing is free. If the cue ball is pocketed,");
            Text x5(700,440," ball in hand is given and, as a penalty,one of the previously");
            Text x6(700,470,"pocketed balls is placed back on the table.");
            Text x7(700,500,"Movement of the ball:");
            Text x8(700,530," Speed and direction is controlled by forcing the ball to move in");
            Text x9(700,560," the click direction and speed depends on the distance of click from cue ball.  ");

            Rectangle r0(100,100,100,40);
            Text x2(100,100,"BACK");
            while(1)
            {
                int clickPos = getClick();
                int cx = clickPos/65536;
                int cy = clickPos % 65536;
                if(50<=cx && cx<=150 &&80<=cy && cy<=120)
                {
                    mode=0;
                    break;
                }
            }
        }
        else if(mode==3)
        {
            Text x1(700,300,highscore);
            Rectangle r0(100,100,100,40);

            Text x2(100,100,"BACK");
            while(1)
            {
                int clickPos = getClick();
                int cx = clickPos/65536;
                int cy = clickPos % 65536;
                if(50<=cx && cx<=150 &&80<=cy && cy<=120)
                {
                    mode=0;
                    break;
                }
            }
        }
        else if(mode==5)
        {
            Text x1(700,300,"SOUND");
            Rectangle Q1(700,300,200,40);

            Text x3(700,400,"LEVEL");
            Rectangle Q2(700,400,200,40);
            Text Z1(800,500,"2.HARD");
            Rectangle Q4(800,500,100,40);
            Rectangle r0(100,100,100,40);
            Text x2(100,100,"BACK");
            Text Z2(600,500,"1.EASY");
            Rectangle Q3(600,500,100,40);


            while(1)
            {
                int clickPos = getClick();
                int cx = clickPos/65536;
                int cy = clickPos % 65536;
                //if clicked on easy the msen that is velocity factor increases otherwise the default mode is hard
                if(cx>=550&&cx<=650&&cy>=480&&cy<=520)
                {
                    msen=30.0;
                }
                if(50<=cx && cx<=150 &&80<=cy && cy<=120)
                {
                    mode=0;
                    break;
                }
            }
        }
        if(mode==4)
        {
            return 0;
        }
        else if(mode==1)
        {
            Text t1(100,100,"Main Menu");

            //code for creating playing area-the table,boundary,balls and pockets.
            Rectangle pt(600,400,800,400);
            pt.setColor(COLOR(0,178,0));
            pt.setFill(true);
            Rectangle r1(600,188,800,24);
            r1.setColor(COLOR(128,0,0));
            r1.setFill(true);
            Rectangle r2(600,612,800,24);
            r2.setColor(COLOR(128,0,0));
            r2.setFill(true);
            Rectangle r3(188,400,24,448);
            r3.setColor(COLOR(128,0,0));
            r3.setFill(true);
            Rectangle r4(1012,400,24,448);
            r4.setColor(COLOR(128,0,0));
            r4.setFill(true);

            Line l1(400,200,400,600);
            l1.setColor(COLOR(255,255,255));
            l1.setFill(true);
            Circle e1(400,400,100);
            e1.setColor(COLOR(255,255,255));
            e1.setFill(false);
            Rectangle a4(452,400,100,300);
            a4.setColor(COLOR(0,178,0));
            a4.setFill(true);

            //target holes
            Circle t[6];
            t[0].init(204,204,20);
            t[1].init(996,204,20);
            t[2].init(204,596,20);
            t[3].init(996,596,20);
            t[4].init(600,200,20);
            t[5].init(600,600,20);
            for(int i=0; i<6; i++)
            {
                t[i].setColor(COLOR(0,0,0));
                t[i].setFill(true);
            }
            Ball b[11];//initialised 11 balls
            //set the ball at required position
            for(int i=0; i<4; i++)
            {
                b[i].SetBall(850,355+30*i);
                b[i].fillcolor(i*i*i,255,255-(i*i*i));
            }
            for(int i=4; i<7; i++)
            {
                b[i].SetBall(850-15*sqrt(3),370+30*(i-4));
                b[i].fillcolor(255,0,0);
            }
            for(int i=7; i<9; i++)
            {
                b[i].SetBall(850-30*sqrt(3),385+30*(i-7));
                b[i].fillcolor(255,150+(10*pow(-1,i)),(10*i));
            }
            b[9].SetBall(850-45*sqrt(3),400);
            b[9].fillcolor(120,0,200);
            b[10].SetBall(400,400);
            b[10].fillcolor(255,255,255);
            //here movement of ball starts
            while(true)
            {
                int start2 = getClick();//gets the coordinates of clicking point
                int start2x=start2/65536;
                int start2y=start2%65536;

                if(50<=start2x && start2x<=150 &&80<=start2y && start2y<=120)//if click on main menu return there
                {
                    mode=0;
                    break;
                }
                //gives velocity to cue ball proportional to distance

                b[10].velocity.x=(start2x-b[10].c.getX())/msen;
                b[10].velocity.y=(start2y-b[10].c.getY())/msen;
                count++;//it keeps count of number of strokes
                while(1)
                {
                    for(int i=0; i<11; i++)
                    {
                        b[i].moveBall();//keeps moving all the balls
                        for(int j=0; j<11; j++)
                        {
                            if(i!=j)
                            {
                                if(b[i].dist_bn_ball(&b[j])<26)
                                {
                                    b[i].Collision(&b[j]);//collision
                                }
                            }
                        }
                        for(int d=0; d<6; d++)//takes the balls pocketed out of the table to(f,0) changes velocity to zero and colours them white
                        {
                            if(sqrt(pow((b[i].c.getX()-t[d].getX()),2)+pow((b[i].c.getY()-t[d].getY()),2))<=22&&i!=10)
                            {
                                ne=i;
                                b[i].SetBall(f,0);
                                b[i].velocity.x=0;
                                b[i].velocity.y=0;
                                b[i].fillcolor(255,255,255);
                                f=f+15;
                            }
                        }
                        for(int d=0; d<6; d++)//if cue ball goes inside the pocket reset it to center and last ball comes in.
                        {
                            if(sqrt(pow((b[10].c.getX()-t[d].getX()),2)+pow((b[10].c.getY()-t[d].getY()),2))<=22)
                            {
                                b[10].SetBall(400,400);
                                b[10].velocity.x=0;
                                b[10].velocity.y=0;
                                b[ne].SetBall(800,400);
                                b[ne].fillcolor(200,62,17);
                            }
                        }
                        if(b[i].velocity.x!=0||b[i].velocity.y!=0)//velocity decreases by friction(.08 times) in resultant unit vector velocity
                        {
                            unitvecx=(b[i].velocity.x)/sqrt(b[i].velocity.x*b[i].velocity.x+b[i].velocity.y*b[i].velocity.y);
                            unitvecy=(b[i].velocity.y)/sqrt(b[i].velocity.x*b[i].velocity.x+b[i].velocity.y*b[i].velocity.y);
                            b[i].velocity.x-=.08*unitvecx;
                            b[i].velocity.y-=.08*unitvecy;
                        }
                        //if velocity(-.5,.5) make it zero
                        if(b[i].velocity.x<.5&&b[i].velocity.x>-.5) b[i].velocity.x=0;
                        if(b[i].velocity.y<.5&&b[i].velocity.y>-.5) b[i].velocity.y=0;
                        //reflection from table boundaries
                        if((b[i].c.getX()>=985&&b[i].velocity.x>0)||(b[i].c.getX()<=215&&b[i].velocity.x<0)) b[i].velocity.x*=-1;
                        if((b[i].c.getY()>=585&&b[i].velocity.y>0)||(b[i].c.getY()<=215&&b[i].velocity.y<0)) b[i].velocity.y*=-1;
                    }
                    if(check_static(b))//checks whether all ball stopped or not so as to prompt user for next click
                    {
                        for(int k=0; k<11; k++)
                        {
                            b[k].velocity.x=0;
                            b[k].velocity.y=0;
                        }
                        break;
                    }
                }
                if(check_end(b))//checks if all the ballls are pocketed or not
                {
                    mode=0;
                    //after game end compares highscore with previous highscore and changes if need
                    ofstream Out("highscore.txt");
                    if(count<highscore)
                    {
                        data=count;
                        Out<<data;
                        Out.close();
                    }
                    cout<<"Congrats!!"<<endl;
                    cout<<"You Have Completed The Game In "<<count<<" Strokes."<<endl;
                    closeCanvas();
                    break;
                }
            }
        }
    }
}
