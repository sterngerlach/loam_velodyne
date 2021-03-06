
// Common.h

// Copyright 2013, Ji Zhang, Carnegie Mellon University
// Further contributions copyright (c) 2016, Southwest Research Institute
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from this
//    software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is an implementation of the algorithm described in the following paper:
//   J. Zhang and S. Singh. LOAM: Lidar Odometry and Mapping in Real-time.
//     Robotics: Science and Systems Conference (RSS). Berkeley, CA, July 2014.

#ifndef LOAM_COMMON_H
#define LOAM_COMMON_H

#include <chrono>
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/filters/filter.h>
#include <pcl/point_types.h>

namespace loam {

/** \brief A standard non-ROS alternative to ros::Time. */
using Time = std::chrono::system_clock::time_point;

/** \brief Construct a new point cloud message from the specified information
 * and publish it via the given publisher.
 *
 * @tparam PointT The point type
 * @param publisher The publisher instance
 * @param cloud The cloud to publish
 * @param stamp The time stamp of the cloud message
 * @param frameId The message frame ID
 */
template <typename PointT>
inline void publishCloudMsg(ros::Publisher& publisher,
                            const pcl::PointCloud<PointT>& cloud,
                            const ros::Time& stamp,
                            const std::string& frameId)
{
    sensor_msgs::PointCloud2 msg;
    pcl::toROSMsg(cloud, msg);
    msg.header.stamp = stamp;
    msg.header.frame_id = frameId;
    publisher.publish(msg);
}

template <typename PointT>
inline void removeNaNFromPointCloud(
    typename pcl::PointCloud<PointT>::Ptr& cloud)
{
    std::vector<int> indices;
    pcl::removeNaNFromPointCloud<PointT>(*cloud, *cloud, indices);
}

inline double toSec(Time::duration duration)
{
    return std::chrono::duration<double>(duration).count();
}

inline Time fromROSTime(const ros::Time& rosTime)
{
    const auto epoch = std::chrono::system_clock::time_point();
    const auto sinceEpoch = std::chrono::seconds(rosTime.sec)
                            + std::chrono::nanoseconds(rosTime.nsec);
    return epoch + sinceEpoch;
}

inline ros::Time toROSTime(const Time& timePoint)
{
    const auto nanoSeconds = std::chrono::duration_cast<
        std::chrono::nanoseconds>(timePoint.time_since_epoch()).count();
    return ros::Time().fromNSec(nanoSeconds);
}

} // namespace loam

#endif // LOAM_COMMON_H
