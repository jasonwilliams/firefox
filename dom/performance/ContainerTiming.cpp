/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "ContainerTiming.h"

#include "Performance.h"
#include "PerformanceMainThread.h"
#include "mozilla/dom/Element.h"
#include "nsContentUtils.h"
#include "nsRFPService.h"

namespace mozilla::dom {

NS_IMPL_CYCLE_COLLECTION_INHERITED(ContainerTiming, PerformanceEntry,
                                   mIntersectionRect, mLastPaintedElement)

NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(ContainerTiming)
NS_INTERFACE_MAP_END_INHERITING(PerformanceEntry)

NS_IMPL_ADDREF_INHERITED(ContainerTiming, PerformanceEntry)
NS_IMPL_RELEASE_INHERITED(ContainerTiming, PerformanceEntry)

ContainerTiming::ContainerTiming(Performance* aPerformance,
                                 const DOMString& aIdentifier,
                                 DOMRectReadOnly* aIntersectionRect,
                                 unsigned long aSize,
                                 DOMHighResTimeStamp aStartTime,
                                 DOMHighResTimeStamp aFirstRenderTime,
                                 Element* aLastPaintedElement)
    : PerformanceEntry(aPerformance->GetParentObject(), u""_ns,
                       nsGkAtoms::container),
      mPerformance(static_cast<PerformanceMainThread*>(aPerformance)),
      mFirstRenderTime(aFirstRenderTime),
      mStartTime(aStartTime),
      mSize(aSize),
      mIntersectionRect(aIntersectionRect),
      mLastPaintedElement(aLastPaintedElement) {
  MOZ_ASSERT(mPerformance);
  MOZ_ASSERT(mLastPaintedElement);

  // The element could be a pseudo-element
  if (mLastPaintedElement->ChromeOnlyAccess()) {
    mLastPaintedElement = Element::FromNodeOrNull(
        aLastPaintedElement->FindFirstNonChromeOnlyAccessContent());
  }
}

JSObject* ContainerTiming::WrapObject(JSContext* aCx,
                                      JS::Handle<JSObject*> aGivenProto) {
  return ContainerTiming_Binding::Wrap(aCx, this, aGivenProto);
}

Element* ContainerTiming::GetLastPaintedElement() const {
  return mLastPaintedElement
             ? nsContentUtils::GetAnElementForTiming(
                   mLastPaintedElement, mLastPaintedElement->GetComposedDoc(),
                   nullptr)
             : nullptr;
}

DOMHighResTimeStamp ContainerTiming::FirstRenderTime() const {
  return nsRFPService::ReduceTimePrecisionAsMSecs(
      mFirstRenderTime,
      static_cast<int64_t>(mPerformance->GetRandomTimelineSeed()),
      mPerformance->GetRTPCallerType());
}

DOMHighResTimeStamp ContainerTiming::StartTime() const {
  return nsRFPService::ReduceTimePrecisionAsMSecs(
      mStartTime, static_cast<int64_t>(mPerformance->GetRandomTimelineSeed()),
      mPerformance->GetRTPCallerType());
}

}  // namespace mozilla::dom
