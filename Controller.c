/*
 *  Controller.c
 *  For the Neural Networks class, this is the only fle you will need to modify.
 *  It contains the agent initialization code and the agent controller code.  An
 *  example of a non-neural controller is included here, which can be modified to
 *
 *
 *  Created by Thomas Caudell on 9/15/09.
 *  Copyright 2009 UNM. All rights reserved.
 *
 */

float wlms11,wlms12,wlms13,wlms14;
float receptor_out[31];
float rvalue,gvalue,bvalue;
float per_intensity=0;
int rotate_flag=1;
int aabb=0;
int aacc=0;
int per_food=0;
int flag2_num=0;
int hold_flag=1;
int lms_flag=0;
int lms_flag2=0;
int lms_flag3=0;
int meta_flag=0;
float temp_bodyangle,temp_bodyfb,temp_bodyrl,temp_bodyth;
int a_hold=0;
int a_hold_flag4=0;
int a_hold_flag3=0;
int a_hold_flag2=0;
int a_hold_flag=0;
int iter_num=0;
int none_food_flag=0;
int count_food=0;
int count_not_food=0;

int single_perceptron19(int input_a,float per_weight);
int single_perceptron18(float input_a, float per_weight);
int single_perceptron17(int input_a,int per_weight);
int single_perceptron16(int input_a,int input_b,int input_c,int per_weight1,int per_weight2,int per_weight3);
int single_perceptron15(int input_a,int input_b,int per_weight1,int per_weight2);
int single_perceptron14(int input_a,int input_b,int input_c,float per_weight1,float per_weight2,float per_weight3);
int single_perceptron13(int input_a, float per_weight);
int single_perceptron12(int coll,float **touch,float per_weight);
int single_perceptron11(int coll,float **touch,float per_weight);
int single_perceptron10(float input_a, float per_weight);
int single_perceptron9(int coll,float **touch,float per_weight);
float single_perceptron8(int coll,float **touch,float per_weight);
int single_perceptron7(float input_a, float per_weight);
int single_perceptron6(int input_a, int per_weight);
int single_perceptron5(float input_a, float per_weight);
int single_perceptron4(int input_a, float per_weight);
int single_perceptron3(float input_a, float per_weight);
int single_perceptron2(float input_a, float per_weight);
int single_perceptron1(int input_a, float per_weight);
int nonezero_perceptron(int input_a,int input_b,int input_c,float per_weight1,float per_weight2,float per_weight3);
int multi_perceptron2(int input_a,int input_b,float per_weight1,float per_weight2,float per_weight3);
int multi_perceptron3(int input_a,int input_b,int input_c,float per_weight1,float per_weight2,float per_weight3,float per_weight4);
int multi_perceptron4(float input_a,float input_b,float input_c);
int multi_perceptron5(int input_a,int input_b,int input_c,float per_weight1,float per_weight2,float per_weight3);
float perceptron_head_angle(float input_a,float out_max,float out_min,float w1);
void process_single_visual_sensor( AGENT_TYPE *a, int *maxrec );
float intensity_perceptron(float input_a,float input_b,float input_c,float per_weight1,float per_weight2,float per_weight3);
float normal_perceptron(float input_a,float input_b,float per_weight1,float per_weight2);
void copy_receptor();
int winner_som(float **eyevalues);


void init(void)
{ /* This function initializes the graphics, and creates and initializes the world an the agent. tpc */

	AGENT_TYPE *agent ;
	GEOMETRIC_SHAPE_TYPE *agentshape  ;
  ACOUSTIC_SHAPE_TYPE *sound ;
	int nsoundreceptors, nsoundbands ;
  float angle_locations0[31] = {-15.,-14.,-13.,-12.,-11.,-10.,-9.,-8.,-7.,-6.,-5.,-4.,-3.,-2.,-1.,0.,1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,13.,14.,15} ;
  float directions0[31] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ;
  time_t now ;
  struct tm *date ;
  char timestamp[30] ;

  extern float wlms11,wlms12,wlms13,wlms14;
  extern float temp_bodyangle,temp_bodyfb,temp_bodyrl,temp_bodyth;

  temp_bodyangle=0;
  temp_bodyfb=0.2;
  temp_bodyrl=0;
  temp_bodyth=0.03;

  wlms11=-0.477262;
  wlms12=1.219367;
  wlms13=-0.157596;
  wlms14=-0.029410;
 /* wlms11=-1.143988;
  wlms12=1.575337;
  wlms13=-0.300356;
  wlms14=-0.029410;*/

  printf("\nepoch_num 0 wlms11: %f  wlms12:  %f  wlms13:  %f  wlms14:  %f\n",wlms11,wlms12,wlms13,wlms14);

	glViewport(0,0,ww,wh) ;
	glMatrixMode( GL_PROJECTION) ;
	glLoadIdentity() ;
	gluPerspective( frustrum_theta,(GLfloat)ww/(GLfloat)wh,frustrum_znear,frustrum_zfar);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;
	glEnable (GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH) ;
	glEnable(GL_LIGHTING) ;

  /* create and initialize the world */
	Flatworld = make_world( 0, 10, 610, 100.0, -100.0, 100.0, -100.0 ) ;
  read_object_spec_file( Flatworld, "WorldObjects.dat" ) ;

	agentshape = read_geometric_shape_file( "geoshapeAgent.dat" ) ;
  sound = read_acoustic_shape_file( "soundshapeAgent.dat" ) ;

  nsoundreceptors = sound->nfrequencies ;
  nsoundbands = sound->nbands ;

  /* Creat and initialize the Agent */
  agent = make_agent( 1, 0.0, 0.0, -30.0, 0.5, 1.0 ) ;
  add_physical_shape_to_agent( agent, agentshape ) ;
  add_sound_shape_to_agent( agent, sound ) ;
  add_visual_sensor_to_agent( agent, 31, 3, 0.0, angle_locations0, directions0 ) ;
  add_acoustic_sensor_to_agent( agent, nsoundreceptors, nsoundbands, 0.0, 90.0 ) ;
  add_acoustic_sensor_to_agent( agent, nsoundreceptors, nsoundbands, 0.0, -90.0 ) ;
  add_cargo_manifest_type_to_agent( agent, 0 ) ;
  add_soma_sensor_to_agent( agent, 1, 0.0, agentshape ) ;
  add_actuators_to_agent( agent, 0.2, 0.0, 0.03, 0.0 ) ;
  set_agent_head_angle( agent, 0.0 ) ;
  set_metabolic_burn_rate_agent(agent, 5.0e-4 ) ;

	add_agent_to_world( Flatworld, agent ) ;

  current_agent = agent ;

  /* Initialize the world and wall clock times. */
  init_world_time( Flatworld ) ;
  now = time(NULL) ;
  date = localtime( &now ) ;
  strftime(timestamp, 30, "%y/%m/%d H: %H M: %M S: %S",date) ;
  printf("init- Start time: %s\n",timestamp) ;
}

int single_perceptron19(int input_a,float per_weight)
{
    int v,d;
    v=input_a*per_weight;
    if(v>380) d=1;
    else d=0;
    return d;
}

int single_perceptron18(float input_a, float per_weight)
{
    int d;
    float temp_value;
    temp_value=input_a*per_weight;
    if(temp_value>0.9) d=1;
    else d=0;
    return d;
}

int single_perceptron17(int input_a,int per_weight)
{
    int v,d;
    v=input_a*per_weight;
    if(v==0) d=1;
    else d=0;
    return d;
}

int single_perceptron16(int input_a,int input_b,int input_c,int per_weight1,int per_weight2,int per_weight3)
{
    extern int flag2_num;
    int temp,v,d;
    float w1,w2,w3;
    w1=1.0;w2=1.0;w3=1.0;

    temp=nonezero_perceptron(input_a,input_b,input_c,w1,w2,w3);
    if(temp==0) v=0;
    else if(input_a==1) v=0*per_weight1;
    else if(input_b==1) v=1*per_weight2;
    else if(input_c==1) v=flag2_num*per_weight3;
    d=v;
    return d;
}

int single_perceptron15(int input_a,int input_b,int per_weight1,int per_weight2)
{
    int d;
    d=input_a*per_weight1*1+input_b*per_weight2*7-input_a*input_b*1;
    return d;
}

int single_perceptron14(int input_a,int input_b,int input_c,float per_weight1,float per_weight2,float per_weight3)
{
    int d;
    float v;

    v=(input_b*per_weight2+input_c*per_weight3)*input_a*per_weight1;
    if(v==1.0) d=1;
    else d=0;

    return d;
}

int single_perceptron13(int input_a, float per_weight)
{
    int d;
    float v;
    v=input_a*per_weight;
    if(v<1) d=1;
    else d=0;
    return d;
}

int single_perceptron10(float input_a, float per_weight)
{
    int d;
    float temp_value;
    temp_value=input_a*per_weight;
    if(temp_value==0.0) d=1;
    else d=0;
    return d;
}

int single_perceptron12(int coll,float **touch,float per_weight)
{
    float w1;
    int d;
    w1=1;

    if(coll>0)
    {
        d=single_perceptron2(*(*(touch+7)+0),w1);
    }
    else d=0;

    return d;
}


int single_perceptron11(int coll,float **touch,float per_weight)
{
    float w1;
    int d;
    w1=1;

    if(coll>0)
    {
        d=single_perceptron2(*(*(touch+1)+0),w1);
    }
    else d=0;

    return d;
}

int single_perceptron9(int coll,float **touch,float per_weight)
{
    float w1;
    int d;
    w1=1;

    if(coll>0)
    {
        d=single_perceptron10(*(*(touch+0)+0),w1);
    }
    else d=0;

    return d;
}

float single_perceptron8(int coll,float **touch,float per_weight)
{
    float d,v;

    if(coll>0)
    {
    v=*(*(touch+0)+0)*per_weight;
    d=v;
    }
    else d=0.0;
    return d;
}

int single_perceptron7(float input_a, float per_weight)
{
    int d;
    float temp_value;
    temp_value=input_a*per_weight;
    if(temp_value>1.5) d=1;
    else d=0;
    return d;
}

int single_perceptron6(int input_a, int per_weight)
{
    int d;
    int temp_value;
    temp_value=input_a*per_weight;
    if(temp_value<1) d=1;
    else d=0;
    return d;
}

int single_perceptron5(float input_a, float per_weight)
{
    int d;
    float temp_value;
    temp_value=input_a*per_weight;
    if(temp_value>160) d=1;
    else d=0;
    return d;
}

int single_perceptron4(int input_a, float per_weight)
{
    int d;
    float temp_value;
    temp_value=input_a*per_weight;
    if(temp_value<0) d=1;
    else d=0;
    return d;
}

int single_perceptron3(float input_a, float per_weight)
{
    int d;
    float temp_value;
    temp_value=input_a*per_weight;
    if(temp_value<=1) d=1;
    else d=0;
    return d;
}

int single_perceptron2(float input_a, float per_weight)
{
    int d;
    float temp_value;
    temp_value=input_a*per_weight;
    if(temp_value>0) d=1;
    else d=0;
    return d;
}

int single_perceptron1(int input_a, float per_weight)
{
    int d;
    float temp_value;
    temp_value=input_a*per_weight;
    if(temp_value>0) d=1;
    else d=0;
    return d;
}

int nonezero_perceptron(int input_a,int input_b,int input_c,float per_weight1,float per_weight2,float per_weight3)
{
    int d;
    float v;
    v=input_a*per_weight1+input_b*per_weight2+input_c*per_weight3;
    if(v>0) d=1;
    else d=0;
    return d;
}

float intensity_perceptron(float input_a,float input_b,float input_c,float per_weight1,float per_weight2,float per_weight3)
{
    float d,v;
    v=sqrt(pow(input_a,2)*per_weight1+pow(input_b,2)*per_weight2+pow(input_c,2)*per_weight3);
    d=v;
    return d;
}

float normal_perceptron(float input_a,float input_b,float per_weight1,float per_weight2)
{
    float d,v,w1;
    w1=1;
    if(input_b==0) v=0;
    else  v=(input_a*per_weight1)/(input_b*per_weight2);
    d=v;
    return d;
}

int multi_perceptron2(int input_a,int input_b,float per_weight1,float per_weight2,float per_weight3)
{
    int d;
    float temp_value;
    temp_value=input_a*per_weight1+input_b*per_weight2+per_weight3;
    if(temp_value>0) d=1;
    else d=0;
    return d;
}

int multi_perceptron3(int input_a,int input_b,int input_c,float per_weight1,float per_weight2,float per_weight3,float per_weight4)
{
    int d;
    float temp_value;
    temp_value=input_a*per_weight1+input_b*per_weight2+input_c*per_weight3+per_weight4;
    if(temp_value>0) d=1;
    else d=0;
    return d;
}

int multi_perceptron4(float input_a,float input_b,float input_c)
{
    extern float wlms11,wlms12,wlms13,wlms14;
    int d;
    float temp_value;
    temp_value=input_a*wlms11+input_b*wlms12+input_c*wlms13+wlms14;
    if(temp_value>=0) d=1;
    else d=0;
    return d;
}

int multi_perceptron5(int input_a,int input_b,int input_c,float per_weight1,float per_weight2,float per_weight3)
{
    int d;
    float temp_value;
    temp_value=input_a*per_weight1+input_b*per_weight2+input_c*per_weight3;
    if(temp_value>=3) d=1;
    else d=0;
    return d;
}

float perceptron_head_angle(float input_a,float out_max,float out_min,float w1)
{
    float d,v;
    v=input_a*w1;
    if(v>=out_max) v=out_max;
    else if(v<=out_min) v=out_min;
    else v=v;
    d=v;
    return d;
}

void copy_receptor(float **eyevalues)
{
    extern float receptor_out[31];
    receptor_out[0]=multi_perceptron4(*(*(eyevalues+0)+0),*(*(eyevalues+0)+1),*(*(eyevalues+0)+2));
    receptor_out[1]=multi_perceptron4(*(*(eyevalues+1)+0),*(*(eyevalues+1)+1),*(*(eyevalues+1)+2));
    receptor_out[2]=multi_perceptron4(*(*(eyevalues+2)+0),*(*(eyevalues+2)+1),*(*(eyevalues+2)+2));
    receptor_out[3]=multi_perceptron4(*(*(eyevalues+3)+0),*(*(eyevalues+3)+1),*(*(eyevalues+3)+2));
    receptor_out[4]=multi_perceptron4(*(*(eyevalues+4)+0),*(*(eyevalues+4)+1),*(*(eyevalues+4)+2));
    receptor_out[5]=multi_perceptron4(*(*(eyevalues+5)+0),*(*(eyevalues+5)+1),*(*(eyevalues+5)+2));
    receptor_out[6]=multi_perceptron4(*(*(eyevalues+6)+0),*(*(eyevalues+6)+1),*(*(eyevalues+6)+2));
    receptor_out[7]=multi_perceptron4(*(*(eyevalues+7)+0),*(*(eyevalues+7)+1),*(*(eyevalues+7)+2));
    receptor_out[8]=multi_perceptron4(*(*(eyevalues+8)+0),*(*(eyevalues+8)+1),*(*(eyevalues+8)+2));
    receptor_out[9]=multi_perceptron4(*(*(eyevalues+9)+0),*(*(eyevalues+9)+1),*(*(eyevalues+9)+2));
    receptor_out[10]=multi_perceptron4(*(*(eyevalues+10)+0),*(*(eyevalues+10)+1),*(*(eyevalues+10)+2));
    receptor_out[11]=multi_perceptron4(*(*(eyevalues+11)+0),*(*(eyevalues+11)+1),*(*(eyevalues+11)+2));
    receptor_out[12]=multi_perceptron4(*(*(eyevalues+12)+0),*(*(eyevalues+12)+1),*(*(eyevalues+12)+2));
    receptor_out[13]=multi_perceptron4(*(*(eyevalues+13)+0),*(*(eyevalues+13)+1),*(*(eyevalues+13)+2));
    receptor_out[14]=multi_perceptron4(*(*(eyevalues+14)+0),*(*(eyevalues+14)+1),*(*(eyevalues+14)+2));
    receptor_out[15]=multi_perceptron4(*(*(eyevalues+15)+0),*(*(eyevalues+15)+1),*(*(eyevalues+15)+2));
    receptor_out[16]=multi_perceptron4(*(*(eyevalues+16)+0),*(*(eyevalues+16)+1),*(*(eyevalues+16)+2));
    receptor_out[17]=multi_perceptron4(*(*(eyevalues+17)+0),*(*(eyevalues+17)+1),*(*(eyevalues+17)+2));
    receptor_out[18]=multi_perceptron4(*(*(eyevalues+18)+0),*(*(eyevalues+18)+1),*(*(eyevalues+18)+2));
    receptor_out[19]=multi_perceptron4(*(*(eyevalues+19)+0),*(*(eyevalues+19)+1),*(*(eyevalues+19)+2));
    receptor_out[20]=multi_perceptron4(*(*(eyevalues+20)+0),*(*(eyevalues+20)+1),*(*(eyevalues+20)+2));
    receptor_out[21]=multi_perceptron4(*(*(eyevalues+21)+0),*(*(eyevalues+21)+1),*(*(eyevalues+21)+2));
    receptor_out[22]=multi_perceptron4(*(*(eyevalues+22)+0),*(*(eyevalues+22)+1),*(*(eyevalues+22)+2));
    receptor_out[23]=multi_perceptron4(*(*(eyevalues+23)+0),*(*(eyevalues+23)+1),*(*(eyevalues+23)+2));
    receptor_out[24]=multi_perceptron4(*(*(eyevalues+24)+0),*(*(eyevalues+24)+1),*(*(eyevalues+24)+2));
    receptor_out[25]=multi_perceptron4(*(*(eyevalues+25)+0),*(*(eyevalues+25)+1),*(*(eyevalues+25)+2));
    receptor_out[26]=multi_perceptron4(*(*(eyevalues+26)+0),*(*(eyevalues+26)+1),*(*(eyevalues+26)+2));
    receptor_out[27]=multi_perceptron4(*(*(eyevalues+27)+0),*(*(eyevalues+27)+1),*(*(eyevalues+27)+2));
    receptor_out[28]=multi_perceptron4(*(*(eyevalues+28)+0),*(*(eyevalues+28)+1),*(*(eyevalues+28)+2));
    receptor_out[29]=multi_perceptron4(*(*(eyevalues+29)+0),*(*(eyevalues+29)+1),*(*(eyevalues+29)+2));
    receptor_out[30]=multi_perceptron4(*(*(eyevalues+30)+0),*(*(eyevalues+30)+1),*(*(eyevalues+30)+2));
}

int winner_som(float **eyevalues)
{
    extern float receptor_out[31];
    extern int rotate_flag;
    extern int aabb,non_food_flag;

    int i,winner_num,temp_num[31],count,per_count;
    float dis1,dis2,max_number,rgb_intensity,w1,w2,w3;
    count=0;max_number=9999;
    winner_num=-1;w1=1;w2=1;w3=1;
    for(i=0;i<31;i++)
    {
        if(receptor_out[i]==1)
        {
            temp_num[count]=i;
            count=count+1;
        }
    }
    per_count=single_perceptron1(count, w1);
    if(per_count==1)
    {
        none_food_flag=0;
        winner_num=temp_num[0];

        rgb_intensity=intensity_perceptron(*(*(eyevalues+temp_num[0])+0),*(*(eyevalues+temp_num[0])+1),*(*(eyevalues+temp_num[0])+2),w1,w2,w3);

        dis1=fabs(max_number-rgb_intensity);
        for(i=0;i<count;i++)
        {
            rgb_intensity=intensity_perceptron(*(*(eyevalues+temp_num[i])+0),*(*(eyevalues+temp_num[i])+1),*(*(eyevalues+temp_num[i])+2),w1,w2,w3);

            dis2=fabs(max_number-rgb_intensity);
            if(dis2<dis1)  winner_num=temp_num[i];
        }
        rotate_flag=1;
        aabb=0;
        aacc=0;
    }
    else if(per_count==0)
    {
        none_food_flag=1;
        rotate_flag=0;

        for(i=0;i<31;i++)
        {
            rgb_intensity=intensity_perceptron(*(*(eyevalues+i)+0),*(*(eyevalues+i)+1),*(*(eyevalues+i)+2),w1,w2,w3);
            if(rgb_intensity>0)
            {
                winner_num=i;
                break;
            }
        }

    }
    return winner_num;
}

void agents_controller( WORLD_TYPE *w )
{ /* Adhoc function to test agents, to be replaced with NN controller. tpc */

  extern float wlms11,wlms12,wlms13,wlms14;

  extern float receptor_out[31];

  OBJECT_TYPE *o ;
  AGENT_TYPE *a ;
  int collision_flag=0 ;
  int k ;
  int maxvisualreceptor = -1 ;
  float dfb , drl, dth, dh ;
  float headthmax, headthmin, headperiod, headth ;
  float objectdirection ;
  float bodyx,bodyy,bodyth, dbodyth ;
  float x, y, h ;
  float /*rfactor = 0.1,*/ inertia=0.2 ;
  float **eyevalues, **ear0values, **ear1values, **touchvalues ;
  time_t now ;
  struct tm *date ;
  char timestamp[30] ;
  float out_d;
  int per_touch=-1;
  int per_obj=-1;
  int per_lms=-1;
  int per_metabolic=-1;
  int per_med=-1;
  int per_scan=-1;
  int per_err=-1;
  int collision_test=-1;
  extern float per_intensity;
  int per_touch0=-1;
  int per_touch1=-1;
  int per_touch7=-1;
  int add_flag=-1;
  extern int rotate_flag;
  int per_aabb=-1;
  int per_angle1=-1;
  int per_aacc=-1;
  int per_angle2=-1;
  int per_touch17=-1;
  int add_flag2=0;
  int wnum1,wnum2,wnum3;
  wnum1=1;wnum2=1;wnum3=1;
  extern int per_food;
  extern int flag2_num,a_hold_flag4,a_hold_flag3,a_hold_flag2,a_hold_flag;
  int per_eat_num1,per_eat_num2;
  extern int hold_flag,lms_flag2,meta_flag,lms_flag3;
  extern float temp_bodyangle,temp_bodyfb,temp_bodyrl,temp_bodyth;
  int iter_flag,turn_head_flag;
  extern int iter_num,none_food_flag;
  turn_head_flag=0;


  float metabolic_previous,metabolic_delta;
  extern int lms_flag;

  float w1,w2,w3;
  float multi_w1,multi_w2,multi_w3,multi_w4;
  multi_w1=2;multi_w2=2;multi_w3=2;multi_w4=-5;
  w1=1;w2=1;w3=1;
  float multi2_w1,multi2_w2,multi2_w3;
  multi2_w1=2;multi2_w2=2;multi2_w3=-3;
  int obj_num=-1;
  float t=-1;

  extern float rvalue,gvalue,bvalue;

  /* Initialize hed scan ranges and scan peroid */
  headthmax = 160.0 ;
  headthmin = -160.0 ;
  headperiod = 100.0 ;

  /* Loop over agents.  For the NN class, there is only one agent */
  for( k=0 ; k<w->nagents ; k++ )
  {

    a = w->agents[k] ; /* get agent pointer */

    /* test if agent is alive. if so, process sensors and actuators.  if not, report death and exit */
    if( a->instate->metabolic_charge>0.0 )
    {

      /* get current muscle rates and body/head angles */

      read_actuators_agent( a, &dfb, &drl, &dth, &dh ) ;
      read_agent_body_position( a, &bodyx, &bodyy, &bodyth ) ;
      read_agent_head_angle( a, &headth ) ;

      /* move the agents body */
         temp_bodyangle=a->outstate->body_angle;
         temp_bodyfb=a->instate->bodyactuators->deltaFB;
         temp_bodyrl=a->instate->bodyactuators->deltaRL;
         temp_bodyth=a->instate->bodyactuators->deltaTH;

          move_body_agent( a ) ;

      /* read somatic(touch) sensor for collision */
      collision_flag = read_soma_sensor(w, a) ;
      if(collision_flag>0 )
      {
        touchvalues = extract_soma_receptor_values_pointer( a ) ;
      }

      /* The following should be replaced with an ALL NEURAL controller */
      iter_num=iter_num+1;

      collision_test=single_perceptron1(collision_flag,w1);

      per_touch0=single_perceptron9(collision_flag,touchvalues,w1);
      per_touch1=single_perceptron11(collision_flag,touchvalues,w1);
      per_touch7=single_perceptron12(collision_flag,touchvalues,w1);
      add_flag=multi_perceptron5(per_touch0,per_touch1,per_touch7,w1,w2,w3);

      flag2_num=single_perceptron15(per_touch1,per_touch7,wnum1,wnum2);

      per_touch17=single_perceptron14(per_touch0,per_touch1,per_touch7,w1,w2,w3);

     t=single_perceptron8(collision_flag,touchvalues,w1);

          per_touch=single_perceptron2(t, w1);

          per_err=single_perceptron7(per_intensity, w1);

          add_flag2=multi_perceptron5(per_touch17,per_food,per_err,w1,w2,w3);

            per_eat_num1=single_perceptron16(per_touch,add_flag,add_flag2,wnum1,wnum2,wnum3);
            per_eat_num2=single_perceptron17(per_eat_num1,wnum1);

      obj_num=(per_touch*a->instate->skin->touched_objects[0]+(1-per_touch))*(1-add_flag)+add_flag*a->instate->skin->touched_objects[1]+\
               add_flag2*a->instate->skin->touched_objects[flag2_num];

              per_obj=single_perceptron1(obj_num,w1);

          per_lms=multi_perceptron2(per_touch,per_obj,multi2_w1,multi2_w2,multi2_w3);

          per_med=multi_perceptron2(per_err,per_lms,multi2_w1,multi2_w2,multi2_w3);

          lms_flag2=nonezero_perceptron(per_med,add_flag,add_flag2,w1,w2,w3);

            o=w->objects[obj_num-1];

            metabolic_previous=a->instate->metabolic_charge;

          meta_flag=single_perceptron18(metabolic_previous, w1);

            lms_flag3=multi_perceptron2(lms_flag2,meta_flag,multi2_w1,multi2_w2,multi2_w3);

            a_hold_flag2=single_perceptron6(a_hold,wnum1);

            a_hold_flag=multi_perceptron2(lms_flag3,a_hold_flag2,multi2_w1,multi2_w2,multi2_w3);

            temp_bodyangle=temp_bodyangle*(1-a_hold_flag)+a->outstate->body_angle*a_hold_flag;
            temp_bodyfb=temp_bodyfb*(1-a_hold_flag)+a->instate->bodyactuators->deltaFB*a_hold_flag;
            temp_bodyrl=temp_bodyrl*(1-a_hold_flag)+a->instate->bodyactuators->deltaRL*a_hold_flag;
            temp_bodyth=temp_bodyth*(1-a_hold_flag)+a->instate->bodyactuators->deltaTH*a_hold_flag;

            a->outstate->body_angle=a->outstate->body_angle*(1-a_hold_flag)+temp_bodyangle*a_hold_flag;
            a->instate->bodyactuators->deltaFB=a->instate->bodyactuators->deltaFB*(1-a_hold_flag);
            a->instate->bodyactuators->deltaRL=a->instate->bodyactuators->deltaRL*(1-a_hold_flag);
            a->instate->bodyactuators->deltaTH=a->instate->bodyactuators->deltaTH*(1-a_hold_flag);

            lms_flag=lms_flag2*(1-meta_flag);
            hold_flag=1-lms_flag2*meta_flag;

            a_hold=a_hold+a_hold_flag;

            a_hold_flag3=multi_perceptron2(hold_flag,lms_flag,multi2_w1,multi2_w2,multi2_w3);
            a_hold_flag4=multi_perceptron2(1-meta_flag,a_hold_flag3,multi2_w1,multi2_w2,multi2_w3);

            a->outstate->body_angle=a->outstate->body_angle*(1-a_hold_flag4)+temp_bodyangle*a_hold_flag4;
            a->instate->bodyactuators->deltaFB=a->instate->bodyactuators->deltaFB*(1-a_hold_flag4)+0.2*a_hold_flag4;
            a->instate->bodyactuators->deltaRL=a->instate->bodyactuators->deltaRL*(1-a_hold_flag4);
            a->instate->bodyactuators->deltaTH=a->instate->bodyactuators->deltaTH*(1-a_hold_flag4)+0.03*a_hold_flag4;

            a_hold=a_hold*(1-a_hold_flag4);

            agent_eat_object_with_flag(  w, a, o ,lms_flag) ;

            iter_num=iter_num*(1-lms_flag);

            iter_flag=single_perceptron19(iter_num,w1);

            turn_head_flag=multi_perceptron2(iter_flag,none_food_flag,multi2_w1,multi2_w2,multi2_w3);

            iter_num=iter_num*(1-turn_head_flag);

            metabolic_delta=a->instate->metabolic_charge-metabolic_previous;
          /*  a->instate->itemp[0]++ ; */ /* increment the total number of objects eaten. this uses one of the user definable
                                         temp variable in the agent data struct. */

            a->instate->itemp[0]=a->instate->itemp[0]+lms_flag;

           if(lms_flag==1)
          {
              if(o->type==1) count_food=count_food+1;
              else count_not_food=count_not_food+1;
            printf("agent_controller- Object of type: %d eaten. New charge: %f total eaten: %d, Food No.: %d, Not Food No.: %d\n",o->type,a->instate->metabolic_charge,a->instate->itemp[0],count_food,count_not_food) ;
          }

            per_metabolic=single_perceptron2(metabolic_delta,w1);

            out_d=per_metabolic;

      /* read visual and sound sensors */
      read_visual_sensor( w, a) ;
      eyevalues = extract_visual_receptor_values_pointer( a, 0 ) ;
      read_acoustic_sensor( w, a) ;
      ear0values = extract_sound_receptor_values_pointer( a, 0 ) ;
      ear1values = extract_sound_receptor_values_pointer( a, 1 ) ;

      per_food=multi_perceptron4(eyevalues[0][0],eyevalues[0][1],eyevalues[0][2]);

      per_intensity=intensity_perceptron(eyevalues[0][0],eyevalues[0][1],eyevalues[0][2],w1,w2,w3);

      rvalue=normal_perceptron(eyevalues[0][0],per_intensity,w1,w2);
      gvalue=normal_perceptron(eyevalues[0][1],per_intensity,w1,w2);
      bvalue=normal_perceptron(eyevalues[0][2],per_intensity,w1,w2);

      copy_receptor(eyevalues);

/* The following should be replaced with an ALL NEURAL controller */
      maxvisualreceptor=winner_som(eyevalues);
 /*     process_single_visual_sensor( a, &maxvisualreceptor ) ;*/
      per_scan=single_perceptron4(maxvisualreceptor,w1);

      per_aabb=single_perceptron13(aabb,w1);
       per_angle1=multi_perceptron2(per_aabb,per_scan,multi2_w1,multi2_w2,multi2_w3);

      set_agent_body_angle( a, (bodyth+163)*per_angle1*hold_flag*(1-turn_head_flag)+(bodyth+163)*turn_head_flag) ;
      aabb=aabb+per_angle1;

        objectdirection = visual_receptor_position( a->instate->eyes[0], maxvisualreceptor*(1-per_scan)) ;

        /* rotate head to center brightest object in center of field of view */
        rotate_agent_head( a, objectdirection*(1-per_scan)*hold_flag ) ;

        /* get new head direction and test if its in bounds of head direction (wrt the body) max and min */
        read_agent_head_angle( a, &headth ) ;
        headth=perceptron_head_angle(headth,headthmax,headthmin,w1)*(1-per_scan)*hold_flag;

        /* enforce head angle being in scan range */

      per_aacc=single_perceptron13(aacc,w1);
      per_angle2=multi_perceptron2(per_aacc,1-per_scan,multi2_w1,multi2_w2,multi2_w3);

         set_agent_head_angle( a, headth*per_angle2 ) ;
         aacc=aacc+per_angle2;

        /* compute change in body direction wrt world that will reduce head direction angle wrt the body.  Inertia causes
           this change in body direction wrt world to be gradual. this action slowly rotates the body to point in the
           same direction the head is looking (wrt world) */
        dbodyth = inertia * headth*(1-per_scan)*hold_flag ;

         set_agent_body_angle(a, (bodyth+dbodyth)*(1-per_scan)+(bodyth+163)*per_scan+(bodyth+163)*turn_head_flag);

      /* decrement metabolic charge by basil metabolism rate.  DO NOT REMOVE THIS CALL */
      basal_metabolism_agent( a ) ;

      /* increment world time clock */
      increment_world_clock( w ) ;

    } /* end agent alive condition */
    else
    {
      /* Example of agent is dead condition */
      printf("agent_controller- Agent has died, eating %d objects.\n",a->instate->itemp[0]) ;
      print_world_time( Flatworld) ;
      now = time(NULL) ;
      date = localtime( &now ) ;
      strftime(timestamp, 30, "%y/%m/%d H: %H M: %M S: %S",date) ;
      printf("Death time: %s\n",timestamp) ;

       exit(0);

      /* Example as to how to restore the world and agent after it dies. */
      restore_objects_to_world( Flatworld ) ;  /* restore all of the objects h=back into the world */
      reset_agent_charge( a ) ;               /* recharge the agent's battery to full */
      a->instate->itemp[0] = 0 ;              /* zero the number of object's eaten accumulator */
      init_world_time( Flatworld ) ;          /* zero the Flatworld clock */
      x = distributions_uniform( Flatworld->xmin, Flatworld->xmax ) ; /* pick random starting position and heading */
      y = distributions_uniform( Flatworld->ymin, Flatworld->ymax ) ;
      h = distributions_uniform( -179.0, 179.0) ;
      set_agent_body_position( a, x, y, h ) ;    /* set new position and heading of agent */

    } /* end agent dead condition */

  } /* end over all agent loop */

}
