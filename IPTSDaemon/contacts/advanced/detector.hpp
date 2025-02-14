#pragma once

#include <common/types.hpp>

#include "algorithm/distance_transform.hpp"
#include "algorithm/gaussian_fitting.hpp"

#include <container/image.hpp>
#include <container/kernel.hpp>

#include <contacts/interface.hpp>

#include <math/vec2.hpp>
#include <math/mat2.hpp>

#include <array>
#include <vector>
#include <queue>

using namespace iptsd::container;
using namespace iptsd::math;


namespace iptsd::contacts::advanced {

struct ComponentStats {
    u32 size;
    f32 volume;
    f32 incoherence;
    u32 maximas;
};


class BlobDetector : public IBlobDetector {
public:
    BlobDetector(index2_t size, BlobDetectorConfig config);

    auto data() -> Image<f32> & override;
    auto search() -> std::vector<Blob> const& override;

private:
    auto process(Image<f32> const& hm) -> std::vector<Blob> const&;

    BlobDetectorConfig config;

    // temporary storage
    Image<f32> m_hm;
    Image<f32> m_img_pp;
    Image<Mat2s<f32>> m_img_m2_1;
    Image<Mat2s<f32>> m_img_m2_2;
    Image<std::array<f32, 2>> m_img_stev;
    Image<f32> m_img_rdg;
    Image<f32> m_img_obj;
    Image<u16> m_img_lbl;
    Image<f32> m_img_dm1;
    Image<f32> m_img_dm2;
    Image<f32> m_img_flt;
    Image<f64> m_img_gftmp;

    std::priority_queue<alg::wdt::QItem<f32>> m_wdt_queue;
    std::vector<alg::gfit::Parameters<f64>> m_gf_params;

    std::vector<index_t> m_maximas;
    std::vector<ComponentStats> m_cstats;
    std::vector<f32> m_cscore;

    // gauss kernels
    Kernel<f32, 5, 5> m_kern_pp;
    Kernel<f32, 5, 5> m_kern_st;
    Kernel<f32, 5, 5> m_kern_hs;

    // parameters
    index2_t m_gf_window;

    // output
    std::vector<Blob> m_touchpoints;
};

inline auto BlobDetector::data() -> Image<f32> &
{
    return m_hm;
}

inline auto BlobDetector::search() -> std::vector<Blob> const&
{
	return this->process(this->m_hm);
}

} /* namespace iptsd::contacts::advanced */
