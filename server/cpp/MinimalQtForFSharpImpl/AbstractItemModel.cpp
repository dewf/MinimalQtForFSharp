#include "generated/AbstractItemModel.h"

#include <QAbstractItemModel>
#include <QModelIndex>

#include "ModelIndexInternal.h"
#include "VariantInternal.h"

#define THIS ((QAbstractItemModel*)_this)

namespace AbstractItemModel
{
    ModelIndex::OwnedHandleRef Handle_index(HandleRef _this, int32_t row, int32_t column) {
        auto value = THIS->index(row, column);
        return (ModelIndex::OwnedHandleRef) new QModelIndex(value);
    }

    ModelIndex::OwnedHandleRef Handle_index(HandleRef _this, int32_t row, int32_t column, std::shared_ptr<ModelIndex::Deferred::Base> parent) {
        auto value = THIS->index(row, column, ModelIndex::fromDeferred(parent));
        return (ModelIndex::OwnedHandleRef) new QModelIndex(value);
    }

    bool Handle_setData(HandleRef _this, std::shared_ptr<ModelIndex::Deferred::Base> index, std::shared_ptr<Variant::Deferred::Base> value) {
        auto index2 = ModelIndex::fromDeferred(index);
        auto value2 = Variant::fromDeferred(value);
        return THIS->setData(index2, value2);
    }

    bool Handle_setData(HandleRef _this, std::shared_ptr<ModelIndex::Deferred::Base> index, std::shared_ptr<Variant::Deferred::Base> value, ItemDataRole role) {
        auto index2 = ModelIndex::fromDeferred(index);
        auto value2 = Variant::fromDeferred(value);
        return THIS->setData(index2, value2, (int)role);
    }
}
