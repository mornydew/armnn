//
// Copyright © 2017 Arm Ltd. All rights reserved.
// SPDX-License-Identifier: MIT
//
#include "L2NormalizationLayer.hpp"

#include "LayerCloneBase.hpp"

#include <armnn/TypesUtils.hpp>
#include <backendsCommon/WorkloadData.hpp>
#include <backendsCommon/WorkloadFactory.hpp>

namespace armnn
{

L2NormalizationLayer::L2NormalizationLayer(const L2NormalizationDescriptor& param, const char* name)
    : LayerWithParameters(1, 1, LayerType::L2Normalization, param, name)
{
}

std::unique_ptr<IWorkload> L2NormalizationLayer::CreateWorkload(const IWorkloadFactory& factory) const
{
    L2NormalizationQueueDescriptor descriptor;
    return factory.CreateL2Normalization(descriptor, PrepInfoAndDesc(descriptor));
}

L2NormalizationLayer* L2NormalizationLayer::Clone(Graph& graph) const
{
    return CloneBase<L2NormalizationLayer>(graph, m_Param, GetName());
}

void L2NormalizationLayer::ValidateTensorShapesFromInputs()
{
    VerifyLayerConnections(1, CHECK_LOCATION());

    auto inferredShapes = InferOutputShapes({ GetInputSlot(0).GetConnection()->GetTensorInfo().GetShape() });

    BOOST_ASSERT(inferredShapes.size() == 1);

    ConditionalThrowIfNotEqual<LayerValidationException>(
        "L2NormalizationLayer: TensorShape set on OutputSlot[0] does not match the inferred shape.",
        GetOutputSlot(0).GetTensorInfo().GetShape(),
        inferredShapes[0]);
}

void L2NormalizationLayer::Accept(ILayerVisitor& visitor) const
{
    visitor.VisitL2NormalizationLayer(this, GetParameters(), GetName());
}

} // namespace armnn
