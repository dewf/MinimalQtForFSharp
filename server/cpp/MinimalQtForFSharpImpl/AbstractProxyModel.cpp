#include "generated/AbstractProxyModel.h"

#include <QAbstractProxyModel>
#include <QModelIndex>

#include "ModelIndexInternal.h"

#define THIS ((QAbstractProxyModel*)_this)

namespace AbstractProxyModel
{
    void Handle_setSourceModel(HandleRef _this, AbstractItemModel::HandleRef sourceModel) {
        THIS->setSourceModel((QAbstractItemModel*)sourceModel);
    }

    ModelIndex::OwnedHandleRef Handle_mapToSource(HandleRef _this, std::shared_ptr<ModelIndex::Deferred::Base> proxyIndex) {
        auto retValue = THIS->mapToSource(ModelIndex::fromDeferred(proxyIndex));
        // most model indexes are pointers to stack-allocated stuff on the C++ side, but this one we are responsible for!
        return (ModelIndex::OwnedHandleRef) new QModelIndex(retValue);
    }
}
