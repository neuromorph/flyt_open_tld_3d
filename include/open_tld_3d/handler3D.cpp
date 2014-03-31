#include "handler3D.hpp"

void handler3D::conversionFROMrosmsg(const sensor_msgs::PointCloud2ConstPtr& cloudy, cv::Mat& result, pcl::PointCloud<pcl::PointXYZRGBA>* cloud){

	pcl::fromROSMsg(*cloudy, *cloud);	
	if (cloud->isOrganized()) {
	    result = cv::Mat(cloud->height, cloud->width, CV_8UC3);
	    
		  for (int h=0; h<result.rows; h++) {
		      for (int w=0; w<result.cols; w++) {
		          pcl::PointXYZRGBA point = cloud->at(w, h);

		          Eigen::Vector3i rgb = point.getRGBVector3i();

		          result.at<cv::Vec3b>(h,w)[0] = rgb[2];
		          result.at<cv::Vec3b>(h,w)[1] = rgb[1];
		          result.at<cv::Vec3b>(h,w)[2] = rgb[0];
		      }
		  }
	}
	else{
		std::cerr << "Cloud not organized, can't apply the function" << std::endl;
	}
}


void handler3D::deepness(cv::Rect *currBB, pcl::PointCloud<pcl::PointXYZRGBA>* cloud){
	if(currBB!=NULL){
		int x=currBB->x+(currBB->width/2);
		int y=currBB->y+(currBB->height/2);
	
		(*this).poseSt.pose.position.x=cloud->at(x,y).z;
	
		(*this).poseSt.pose.orientation.x=0;
		(*this).poseSt.pose.orientation.y=0;
		(*this).poseSt.pose.orientation.z=0;
		(*this).poseSt.pose.orientation.w=1;
	
		(*this).poseSt.header.stamp=ros::Time::now();
		(*this).poseSt.header.frame_id="/base_link";
		(*this).publish();
	}
}

void handler3D::publish(){
	(*this).pilote.publish(poseSt);
}
