#ifndef TARGET_MANAGER_H
#define TARGET_MANAGER_H

#include "traj_gen/PolyTrajGen.h"
#include "chomp_predict/chomp_predict.h"
#include <tf/transform_broadcaster.h>

/**
 * @brief Target manager to be used in no-prediction mode (prior target path is precomputed before simulation)
 * 
 */
class TargetManager{

    private:
        PathPlanner planner;
        string target_frame_id;
        string world_frame_id;
        double min_z; 
        ros::Publisher pub_marker_waypoints;
        ros::Publisher pub_path;
        ros::Subscriber sub_waypoints;
        tf::TransformBroadcaster* br_ptr;
        visualization_msgs::MarkerArray wpnt_markerArray;

        // current information after initialization 
        nav_msgs::Path global_path;        
        nav_msgs::Path waypoints_seq;
    
        TrajGenOpts traj_option; // trajectory generation option 

    public:

        vector<geometry_msgs::PoseStamped> queue; // waypoints for generating the global trajectory 

        // flags 
        bool is_insert_permit = false;
        bool is_path = false; // was path computed
        double cur_spline_eval_time = 0;
        double previous_elapsed =0; // before publish control button pressed 
        double button_elapsed=0; // after the button pressed again,

        // member functions
        TargetManager();
        void broadcast_target_tf(double time);
        void init(ros::NodeHandle nh);
        void session(double t_eval);
        void queue_file_load(vector<geometry_msgs::PoseStamped>& wpnt_replace);
        void pop_waypoint();
        void clear_waypoint();
        bool global_path_generate(double tf);
        void callback_waypoint(const geometry_msgs::PoseStampedConstPtr& waypoint);
        void queue_file_load(int,vector<geometry_msgs::PoseStamped>&);
        nav_msgs::Path get_global_waypoints();
        vector<Point> eval_time_seq(VectorXd ts);    
        
};
 
 /**
  * @brief Target predictor in prediction mode
  * 
  */
class TargetPredictor{
    private:
    /**
     * @brief chomp forecast for target prediction  
     * Forecaster was defined as a point to avoid creating nodehandle before ros::init
     */
        CHOMP::ChompForecaster* forecaster_ptr; 
    public: 
      
        TargetPredictor();
        void init(); 
        vector<Point> eval_time_seq(vector<ros::Time> ts);    
        CHOMP::ChompForecaster* get_forecaster_ptr();
};

#endif